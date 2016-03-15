package fighterPilot.RisingSunAlpha;







import com.google.android.vending.expansion.downloader.impl.DownloaderService;

import fighterPilot.RisingSunAlpha.GoogleAlarmReceiver;







public class GoogleDownloadService extends DownloaderService {
	
    // You must use the public key belonging to your publisher account
    public static final String BASE64_PUBLIC_KEY = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAmTHbOr3EuIZddu5wAioLWK5+wwI9GX68bpBdC+8hdRbbanV9F1l4u6kZl/X2BfRbycoVSkYkkGDAhzFOdm98ZRSdWYEkHHhFvelFGweZdmqA/llEFj99X70g9a2FHkyKJzvLz2zkm3qvRp+fZyHSsUq0fZoBvPUnNoxoXeXxz5qvQ1RCyA2TX52HRPwprYJj6Q4Vh5BIoxYZxxTC31rs++dD45BlVLLyiKuVTIfnA/xYJ5ZFJ2n5Av4AOggBazv8UxMYyPOqHpPFuw0Ld9I9jdWsvwpABsR/vEnYUioWthiORz7p9o+aHYnqjje4/MlJEa7nW5DGx0cLrmwiIdlrCwIDAQAB";
    
    
    // You should also modify this salt
    public static final byte[] SALT = new byte[] { 3, 42, -15, -1, 54, 23,
            100, 88, 43, 5, -8, -4, 9, 5, -106, -107, -33, 22, -1, 11
    };

    @Override
    public String getPublicKey() {
        return BASE64_PUBLIC_KEY;
    }

    @Override
    public byte[] getSALT() {
        return SALT;
    }

    @Override
    public String getAlarmReceiverClassName() {
        return GoogleAlarmReceiver.class.getName();
    }
	
	

}
