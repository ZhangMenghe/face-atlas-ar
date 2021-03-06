package face.atlas;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLUtils;
import android.util.Log;

import java.io.IOException;

public class JNIInterface {
    private static final String TAG = "JNIInterface";
    static AssetManager assetManager;

    public static native long JNIonCreate(AssetManager asset_manager);
    public static native void JNIonGlSurfaceCreated();
    public static native void JNIonPause();
    public static native void JNIonResume(Context context, Activity activity);
    public static native void JNIonDestroy();
    public static native void JNIonSurfaceChanged(int rotation, int width, int height);
    public static native void JNIdrawFrame();

    public static Bitmap loadImage(String imageName) {
        try {
            return BitmapFactory.decodeStream(assetManager.open(imageName));
        } catch (IOException e) {
            Log.e(TAG, "Cannot open image " + imageName);
            return null;
        }
    }

    public static void loadTexture(int target, Bitmap bitmap) {
        GLUtils.texImage2D(target, 0, bitmap, 0);
    }
}