package com.tes.project.application;


import android.app.Application;
import android.content.SharedPreferences;

public class App extends Application {
    private static App context;

    public static App getContext() {
        return context;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        context = this;
        // 全局异常初始化
        //CrashHandler.getInstance().init(this);
//        CrashUtils.init();
        // 初始化Utils
        com.tes.infrastructure.utils.Utils.init(this);


    }

    /**
     * 获取设置
     *
     * @param name
     * @return
     */
    public static String getSetting(String name) {
        SharedPreferences shared = context.getSharedPreferences("settings", 0);
        return shared.getString(name, "");
    }

    /**
     * 保存设置
     *
     * @param name
     * @param value
     */
    public static void setSetting(String name, String value) {
        SharedPreferences shared = context.getSharedPreferences("settings", 0);
        SharedPreferences.Editor editor = shared.edit();
        editor.putString(name, value);
        editor.apply();
    }

    /**
     * 保存设置
     *
     * @param name
     * @param value
     */
    public static void setSetting(String name, int value) {
        SharedPreferences shared = context.getSharedPreferences("settings", 0);
        SharedPreferences.Editor editor = shared.edit();
        editor.putInt(name, value);
        editor.apply();
    }

    /**
     * 获取缓存
     *
     * @param name
     * @return
     */
    public static String getCache(String name) {
        SharedPreferences shared = context.getSharedPreferences("cache", 0);
        return shared.getString(name, "");
    }

    /**
     * 保存缓存
     *
     * @param name
     * @param value
     */
    public static void setCache(String name, String value) {
        SharedPreferences shared = context.getSharedPreferences("cache", 0);
        SharedPreferences.Editor editor = shared.edit();
        editor.putString(name, value);
        editor.apply();
    }

    /**
     * 保存缓存
     *
     * @param name
     * @param value
     */
    public static void setCache(String name, int value) {
        SharedPreferences shared = context.getSharedPreferences("cache", 0);
        SharedPreferences.Editor editor = shared.edit();
        editor.putInt(name, value);
        editor.apply();
    }

}
