package com.tes.project.utils;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;

import java.security.MessageDigest;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class WychUtils {


    public static long getApkCRC(Context context) {
        ZipFile zf;
        try {
            zf = new ZipFile(context.getPackageCodePath());
            ZipEntry ze = zf.getEntry("classes.dex");
            return ze.getCrc();
        } catch (Exception ex) {
            return 0;
        }
    }


    public static String getSignature(Context context) {

        try {

            StringBuilder sb = new StringBuilder();
            PackageInfo pi = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
            for (Signature signature : pi.signatures) {
                sb.append(signature.toCharsString());
            }
           return  sb.toString();
        } catch (
                PackageManager.NameNotFoundException e) {
           return "";
        }
    }

    //字符串加密
    public static String getMD5(String info) {
        try {
            MessageDigest md5 = MessageDigest.getInstance("MD5");
            md5.update(info.getBytes("utf-8"));
            byte[] encryption = md5.digest();
            StringBuffer stringBuffer = new StringBuffer();
            for (int i = 0; i < encryption.length; i++) {
                if (Integer.toHexString(0xff & encryption[i]).length() == 1) {
                    stringBuffer.append("0").append(Integer.toHexString(0xff & encryption[i]));
                } else {
                    stringBuffer.append(Integer.toHexString(0xff & encryption[i]));
                }
            }
            return stringBuffer.toString();
        } catch (Exception e) {
//            e.printStackTrace();
            return "";
        }
    }


}
