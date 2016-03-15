/*
 * AndroidGraphicsEngine.cpp
 *
 *  Created on: Jun 5, 2012
 *      Author: dstoll
 */
#include "includes/native/CAndroidFighterPilotThePacificWar.h"
#include "includes/native/graphic/AndroidGraphicsEngine.h"
#include "includes/core/graphic/CGraphicsEngine.h"
#include "includes/native/utility/CAndroidImageStore.h"
//get the a native reference
#include "android/native_window_jni.h"

jobject CAndroidGraphicsEngine::JavaAndroidGraphicsEngineObject;

void CAndroidGraphicsEngine::InitJavaAndroidGraphicsEngine(JNIEnv* aJniEnv)
{
	CAndroidFighterPilotThePacificWar::CreateJavaObjectForVM(aJniEnv,"fighterPilot/RisingSunAlpha/AndroidGameThread", &CAndroidGraphicsEngine::JavaAndroidGraphicsEngineObject);
}

NativeWindow CGraphicsEngine::GetNativeWindow()
{
	//get JNI
	JNIEnv* lJniEnv= CAndroidFighterPilotThePacificWar::GetJNI(); //need reference to the JNI
	//get IDs
	jclass lActivityClass = lJniEnv->GetObjectClass(CAndroidGraphicsEngine::JavaAndroidGraphicsEngineObject);
	jmethodID lGetWindowSurfaceMethod = lJniEnv->GetStaticMethodID(lActivityClass, "GetGameSurface", "()Landroid/view/Surface;");

	//call the method
	jobject lJavaSurface = (jobject) lJniEnv->CallStaticObjectMethod(lActivityClass, lGetWindowSurfaceMethod);

	ANativeWindow* lNativeWindow = ANativeWindow_fromSurface(lJniEnv, lJavaSurface);

	//cleanup
	lJniEnv->DeleteLocalRef(lActivityClass);
	lJniEnv->DeleteLocalRef(lJavaSurface);

	return lNativeWindow;
}

SBitmap CGraphicsEngine::LoadBitmapImageFromDisk(const char* aPath)
{
	SBitmap lLoadedBitmap;

	//need reference to the JNI
	JNIEnv* lJniEnv= CAndroidFighterPilotThePacificWar::GetJNI();

	//get IDs
	//A Bit messed up since I first put it into the image Java class...
	jclass lImageClass = lJniEnv->GetObjectClass(CAndroidImageStore::JavaImageStoreObject);
	jmethodID lOpenBitmapMethod = lJniEnv->GetStaticMethodID(lImageClass, "OpenBitmap", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
	jmethodID lGetWidthMethod = lJniEnv->GetStaticMethodID(lImageClass, "GetWidth", "(Landroid/graphics/Bitmap;)I");
	jmethodID lGetHeightMethod = lJniEnv->GetStaticMethodID(lImageClass, "GetHeight", "(Landroid/graphics/Bitmap;)I");
	jmethodID lGetPixelDataMethod = lJniEnv->GetStaticMethodID(lImageClass, "GetPixelData", "(Landroid/graphics/Bitmap;[I)V");
	jmethodID lCloseMethod = lJniEnv->GetStaticMethodID(lImageClass, "CloseBitmap", "(Landroid/graphics/Bitmap;)V");

	//get Bitmap object
	jstring lJavaString = lJniEnv->NewStringUTF(aPath);
	jobject lBitmap = (jobject) lJniEnv->CallStaticObjectMethod(lImageClass, lOpenBitmapMethod, lJavaString);
	if(lBitmap != NULL) //might fail if not enough memory
	{
		//get dimension of bitmap
		lLoadedBitmap.WidthInPixels = (TInt) lJniEnv->CallStaticIntMethod(lImageClass, lGetWidthMethod, lBitmap);
		lLoadedBitmap.HeightInPixels = (TInt) lJniEnv->CallStaticIntMethod(lImageClass, lGetHeightMethod, lBitmap);

		//get Pixel data
		lLoadedBitmap.Data = new TUint8[sizeof(TUint) * lLoadedBitmap.WidthInPixels * lLoadedBitmap.HeightInPixels];
		memset(lLoadedBitmap.Data, 0, sizeof(TUint) * lLoadedBitmap.WidthInPixels * lLoadedBitmap.HeightInPixels); //zero out array, only important if it fails
		jintArray lJavaIntArray = lJniEnv->NewIntArray(lLoadedBitmap.WidthInPixels * lLoadedBitmap.HeightInPixels);

		if(lJavaIntArray != NULL)
		{
			lJniEnv->CallStaticVoidMethod(lImageClass, lGetPixelDataMethod, lBitmap, lJavaIntArray);

			//need to make a copy of the array
			TUint8* lJavaCopy = (TUint8*) lJniEnv->GetIntArrayElements(lJavaIntArray, 0);
			memcpy(lLoadedBitmap.Data, lJavaCopy, sizeof(TUint) * lLoadedBitmap.WidthInPixels * lLoadedBitmap.HeightInPixels);
			lJniEnv->ReleaseIntArrayElements(lJavaIntArray, (jint*) lJavaCopy, 0); //release the array
		}

		//clean up the image
		lJniEnv->CallStaticVoidMethod(lImageClass, lCloseMethod, lBitmap);

		//release references
		lJniEnv->DeleteLocalRef(lBitmap);
		lJniEnv->DeleteLocalRef(lJavaIntArray);
	}
	else
	{
		//create a debug image, assume enough memory for that
		lLoadedBitmap.Data = new TUint8[sizeof(TUint) * 100 * 100];
		memset(lLoadedBitmap.Data, 0, sizeof(TUint) * 100 * 100);
		lLoadedBitmap.WidthInPixels = 100;
		lLoadedBitmap.HeightInPixels = 100;
	}

	//release references
	lJniEnv->DeleteLocalRef(lImageClass);
	lJniEnv->DeleteLocalRef(lJavaString);

	return lLoadedBitmap;
}



