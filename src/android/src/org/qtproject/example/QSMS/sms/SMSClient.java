package org.qtproject.example.QSMS.sms;

import java.sql.Date;
import java.text.SimpleDateFormat;

import android.app.Activity;
import android.app.PendingIntent;
import android.database.Cursor;
import android.database.ContentObserver;
import android.database.sqlite.SQLiteException;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.provider.ContactsContract;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.provider.ContactsContract.CommonDataKinds.Photo;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import android.telephony.SmsManager;
import android.telephony.SmsMessage;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ContentValues;
import android.content.BroadcastReceiver;
import android.content.AsyncQueryHandler;
import org.qtproject.example.QSMS.R;
import android.content.ComponentName;
import android.widget.Toast;
import java.io.InputStream;
import java.io.ByteArrayOutputStream;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.content.ContentUris;
import android.provider.ContactsContract;

/**
  * 获取所有短息记录：
  * _id            => 短消息序号 如100
  * thread_id      => 对话的序号 如100
  * address        => 发件人地址，手机号.如+8613811810000
  * person         => 发件人，返回一个数字就是联系人列表里的序号，陌生人为null
  * date           => 日期  long型。如1256539465022
  * protocol       => 协议 0 SMS_RPOTO, 1 MMS_PROTO
  * read           => 是否阅读 0未读， 1已读
  * status         => 状态 -1接收，0 complete, 64 pending, 128 failed
  * type           => 类型 1是接收到的，2是已发出
  * body           => 短消息内容
  * service_center => 短信服务中心号码编号。如+8613800755500
  */

public class SMSClient extends BroadcastReceiver
{

    static long qtObject = 0;
    private static Activity m_activity = null;

    public SMSClient()
    { }

    static public void setActivity(Activity activity)
    {
        m_activity = activity;
    }

    static public void hideClient()
    {
        Intent i= new Intent(Intent.ACTION_MAIN);
        i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        i.addCategory(Intent.CATEGORY_HOME);
        m_activity.startActivity(i);
    }

    public native void onMsgReceived(long qtObject, long threadid);
    public native void onMsgSendOut(long qtObject, long msgid, boolean isSuccess);
    public native void onMsgDelivered(long qtObject, long msgid);
    public native void onMsgConversionCreated(long qtObject, long threadid);

    final String SENT_SMS_ACTION = "SENT_SMS_ACTION";
    final String DELIVERED_SMS_ACTION = "DELIVERED_SMS_ACTION";
    final static String RECEIVE_SMS_ACTION = "android.provider.Telephony.SMS_RECEIVED";
    final static String RECEIVE_SMS_ACTION2 = "android.provider.Telephony.SMS_DELIVER";

    public void onReceive(Context context, Intent intent)
    {
        if (intent.getAction().equals("android.provider.Telephony.SMS_DELIVER")
            || intent.getAction().equals("android.provider.Telephony.SMS_RECEIVED") )
        {
            if (qtObject == 0)
                return ;

            onMsgReceived(qtObject, 0);
        }
    }

    private BroadcastReceiver sendMessage = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent)
        {
            synchronized (m_activity) {
                if (qtObject == 0)
                    return;

                boolean sendSuccessed = (getResultCode() == Activity.RESULT_OK ? true : false);
                onMsgSendOut(qtObject, 0, sendSuccessed);
                Toast.makeText(m_activity, "短信发送" + (sendSuccessed ? "成功" : "失败"), Toast.LENGTH_LONG).show();
            }
        }
    };

    private BroadcastReceiver receiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent)
        {
            synchronized (m_activity) {
                if (qtObject == 0)
                    return;
                onMsgDelivered(qtObject, 0);
                Toast.makeText(m_activity, "对方已经接收到短信", Toast.LENGTH_LONG).show();
            }
        }
    };

    public String sendMessage(long threadid, String phoneNumber, String message)
    {
        SmsManager sms = SmsManager.getDefault();

        // set the sms state listener
        Intent sentIntent = new Intent(SENT_SMS_ACTION);
        PendingIntent sentPI = PendingIntent.getBroadcast(m_activity, 0, sentIntent, 0);

        Intent deliverIntent = new Intent(DELIVERED_SMS_ACTION);
        PendingIntent deliverPI = PendingIntent.getBroadcast(m_activity, 0, deliverIntent, 0);

        // split message
        if (message.length() > 70) {
            ArrayList<String> msgs = sms.divideMessage(message);
            for (String msg : msgs) {
                sms.sendTextMessage(phoneNumber, null, msg, sentPI, deliverPI);
            }
        } else {
            sms.sendTextMessage(phoneNumber, null, message, sentPI, deliverPI);
        }

        m_activity.registerReceiver(sendMessage, new IntentFilter(SENT_SMS_ACTION));
        m_activity.registerReceiver(receiver, new IntentFilter(DELIVERED_SMS_ACTION));

        // write current message into sms db
        long timeStamp = System.currentTimeMillis();
        ContentValues values = new ContentValues();
        values.put("date", timeStamp);
        values.put("read", 0);
        values.put("type", 2);
        values.put("address", phoneNumber);
        values.put("body", message);
        values.put("protocol", 0);
        values.put("status", 64);

        m_activity.getContentResolver().insert(Uri.parse("content://sms/sent"), values);

        final String SMS_URI_SENT = "content://sms/sent";
        StringBuilder dataBuilder = new StringBuilder();

        try {
            Uri uri = Uri.parse(SMS_URI_SENT);
            String[] projection = new String[] { "_id", "address", "person", "body",
                                                 "date", "type", "protocol", "read", "status", "thread_id" };
            Cursor cursor = m_activity.getContentResolver().query(uri, projection, "date = " + timeStamp, null, "date desc");

            if (cursor.moveToFirst()) {

                if (threadid >= 0 && threadid == cursor.getLong(9)) {
                    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                    Date d = new Date(cursor.getLong(4));
                    String strDate = dateFormat.format(d);

                    dataBuilder.append(cursor.getLong(0));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getString(1));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getString(2));
                    dataBuilder.append(",");
                    dataBuilder.append(Base64Util.encodeString(cursor.getString(3)));
                    dataBuilder.append(",");
                    dataBuilder.append(strDate);
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(5));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(6));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(7));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(8));
                    dataBuilder.append("\n");
                }

                if (qtObject != 0 && threadid < 0)
                    onMsgConversionCreated(qtObject, cursor.getLong(9));

                if (!cursor.isClosed()) {
                    cursor.close();
                    cursor = null;
                }
            }

        } catch (SQLiteException ex) {
            Log.d("SQLiteException in readConversionData", ex.getMessage());
        }

        return dataBuilder.toString();
    }

    static public int conversionListCount()
    {
        final Uri MMSSMS_FULL_CONVERSATION_URI = Uri.parse("content://mms-sms/conversations");
        final Uri CONVERSATION_URI = MMSSMS_FULL_CONVERSATION_URI.buildUpon().
                appendQueryParameter("simple", "true").build();

        final String[] ALL_THREADS_PROJECTION = { "_id" };

        int count = 0;

        try {
            Uri uri = Uri.parse(CONVERSATION_URI.toString());
            Cursor cursor = m_activity.getContentResolver().query(uri, ALL_THREADS_PROJECTION, null, null, null);

            count = cursor.getCount();

            if (!cursor.isClosed()) {
                cursor.close();
                cursor = null;
            }
        }
        catch (SQLiteException ex) {
            Log.d("SQLiteException in getSmsInPhone", ex.getMessage());
        }

        return count;
    }

//    static public String readContactPhoto(String photoids)
//    {
//        StringBuilder dataBuilder = new StringBuilder();

//        try {
//            final String[] PROJECTION = { ContactsContract.Data._ID, ContactsContract.Data.DATA15 };
//            Cursor photoCursor = m_activity.getContentResolver().query(ContactsContract.Contacts.CONTENT_URI, PROJECTION, ContactsContract.Data._ID + " IN (" + photoids + ")", null, null);

//            if (photoCursor.moveToFirst()) {

//                do {
//                    dataBuilder.append(photoCursor.getLong(0));
//                    dataBuilder.append(",");
//                    dataBuilder.append(Base64Util.encode(photoCursor.getBlob(1)).replace("\n", ""));
//                    dataBuilder.append("\n");
//                }
//                while (photoCursor.moveToNext());
//            }

//            if (!photoCursor.isClosed()) {
//                photoCursor.close();
//                photoCursor = null;
//            }
//       }
//       catch (SQLiteException ex) {
//           Log.d("SQLiteException in readContactPhoto", ex.getMessage());
//       }

//       return dataBuilder.toString();
//    }

    static public String readContactInfo(String recipient_ids)
    {

        HashMap recipientIdMap = new HashMap();

        StringBuilder dataBuilder = new StringBuilder();

        StringBuilder phoneNumBuilder = new StringBuilder();

        // read address of conversion list item
        try {

            final String[] PROJECTION = { "_id", "address" };

            Uri uri = Uri.parse("content://mms-sms/canonical-addresses");
            Cursor cursor = m_activity.getContentResolver().query(uri, PROJECTION, "_id IN (" + recipient_ids + ")", null, null);

            if (cursor.moveToFirst()) {

                int cnter = 0;

                do {

                    String phoneNum = cursor.getString(1);


                    recipientIdMap.put(phoneNum, cursor.getLong(0));
                    if (cnter > 0) phoneNumBuilder.append(",");
                    phoneNumBuilder.append("'");
                    phoneNumBuilder.append(phoneNum);
                    phoneNumBuilder.append("'");
                    cnter++;

                    if (phoneNum.startsWith("+86"))
                        phoneNum = phoneNum.replace("+86", "");
                    else
                        phoneNum = "+86" + phoneNum;

                    recipientIdMap.put(phoneNum, cursor.getLong(0));
                    phoneNumBuilder.append(",");
                    phoneNumBuilder.append("'");
                    phoneNumBuilder.append(phoneNum);
                    phoneNumBuilder.append("'");

//                    // thread id, contact id, user name, phone num, photo id
//                    dataBuilder.append(cursor.getLong(0));
//                    dataBuilder.append(",");
//                    dataBuilder.append(readContactByAddress(cursor.getString(1)));
//                    dataBuilder.append("\n");

                } while (cursor.moveToNext());

                if (!cursor.isClosed()) {
                    cursor.close();
                    cursor = null;
                }
            }
        }
        catch (SQLiteException ex) {
            Log.d("SQLiteException in readContactInfo", ex.getMessage());
        }

        // read contact info by address
        final String[] PHONES_PROJECTION = new String[] { Phone.DISPLAY_NAME, Phone.NUMBER, Photo.PHOTO_ID, Phone.CONTACT_ID };

        try {

            Cursor userCursor = m_activity.getContentResolver().query(Phone.CONTENT_URI,PHONES_PROJECTION,
                            Phone.NUMBER + " IN (" + phoneNumBuilder.toString() + ")", null, null);

            if (userCursor.moveToFirst()) {

                do {

                    // thread id, contact id, user name, phone num, photo id
                    dataBuilder.append(recipientIdMap.get(userCursor.getString(1)).toString());
                    dataBuilder.append(",");
                    dataBuilder.append(userCursor.getString(3));
                    dataBuilder.append(",");
                    dataBuilder.append(userCursor.getString(0));
                    dataBuilder.append(",");
                    dataBuilder.append(userCursor.getString(1));
                    dataBuilder.append(",");
                    dataBuilder.append(userCursor.getLong(2));
                    dataBuilder.append(",");

                    if(userCursor.getLong(2) > 0 ) {
                        Uri uri = ContentUris.withAppendedId(ContactsContract.Contacts.CONTENT_URI, userCursor.getLong(3));
                        InputStream input = ContactsContract.Contacts.openContactPhotoInputStream(m_activity.getContentResolver(), uri);
                        Bitmap contactPhoto = BitmapFactory.decodeStream(input);
                        final ByteArrayOutputStream os = new ByteArrayOutputStream();
                        contactPhoto.compress(Bitmap.CompressFormat.PNG, 100, os);
                        dataBuilder.append(Base64Util.encode(os.toByteArray()).replace("\n", ""));
                    }

                    dataBuilder.append("\n");

                    recipientIdMap.remove(userCursor.getString(1));

                } while (userCursor.moveToNext());
            }
        }
        catch (SQLiteException ex) {
            Log.d("SQLiteException in readContactInfo", ex.getMessage());
        }

        if (!recipientIdMap.isEmpty()) {
            for(Object num : recipientIdMap.keySet()){
                // thread id, contact id, user name, phone num, photo id
                dataBuilder.append(recipientIdMap.get(num).toString());
                dataBuilder.append(",");
                dataBuilder.append(-1);
                dataBuilder.append(",,");
                dataBuilder.append(num.toString());
                dataBuilder.append(",\n");
            }
        }

        return dataBuilder.toString();
    }

    static public String readContactList()
    {
        StringBuilder dataBuilder = new StringBuilder();

        // get phone number
        try {

            HashMap headImgMap = new HashMap();

            Cursor phone = m_activity.getContentResolver().query(Phone.CONTENT_URI,
                    new String[] { Phone.DISPLAY_NAME, Phone.NUMBER, Photo.PHOTO_ID, Phone.CONTACT_ID },
                          null, null, Phone.DISPLAY_NAME + " asc");

            if (phone.moveToFirst()) {

                do {
                    // id, name, number, photo id, photo \n
                    dataBuilder.append(phone.getLong(3));
                    dataBuilder.append(",");
                    dataBuilder.append(phone.getString(0));
                    dataBuilder.append(",");
                    dataBuilder.append(phone.getString(1));
                    dataBuilder.append(",");
                    dataBuilder.append(phone.getLong(2));
                    dataBuilder.append(",");

                    if(phone.getLong(2) > 0 ) {

                        if (!headImgMap.containsKey(phone.getLong(2))) {
                            Uri uri = ContentUris.withAppendedId(ContactsContract.Contacts.CONTENT_URI, phone.getLong(3));
                            InputStream input = ContactsContract.Contacts.openContactPhotoInputStream(m_activity.getContentResolver(), uri);
                            Bitmap contactPhoto = BitmapFactory.decodeStream(input);
                            final ByteArrayOutputStream os = new ByteArrayOutputStream();
                            contactPhoto.compress(Bitmap.CompressFormat.PNG, 100, os);

                            dataBuilder.append(Base64Util.encode(os.toByteArray()).replace("\n", ""));

                            headImgMap.put(phone.getLong(2), Base64Util.encode(os.toByteArray()).replace("\n", ""));
                        }

                        dataBuilder.append(headImgMap.get(phone.getLong(2)));
                    }

                    dataBuilder.append("\n");
                }
                while (phone.moveToNext());

            }

            if (!phone.isClosed()) {
                phone.close();
                phone = null;
            }

        }
        catch (SQLiteException ex) {
            Log.d("SQLiteException in readContactInfo", ex.getMessage());
        }

        return dataBuilder.toString();
    }

    static public void deleteSmsByConversionId(String ids)
    {
        final Uri MMSSMS_FULL_CONVERSATION_URI = Uri.parse("content://mms-sms/conversations");
        m_activity.getContentResolver().delete(MMSSMS_FULL_CONVERSATION_URI, "_id IN ("+ ids +")", null);

        final Uri SMS_URI_ALL = Uri.parse("content://sms");
        m_activity.getContentResolver().delete(SMS_URI_ALL, "thread_id IN ("+ ids +")", null);
    }

    static public void deleteSmsById(String ids)
    {
        final Uri SMS_URI_ALL = Uri.parse("content://sms");
        m_activity.getContentResolver().delete(SMS_URI_ALL, "_id IN ("+ ids +")", null);
    }

    static public String readConversionList(int from, int count)
    {
        final Uri MMSSMS_FULL_CONVERSATION_URI = Uri.parse("content://mms-sms/conversations");
        final Uri CONVERSATION_URI = MMSSMS_FULL_CONVERSATION_URI.buildUpon().
                appendQueryParameter("simple", "true").build();

        final String[] ALL_THREADS_PROJECTION = {
                "_id", "date", "message_count", "recipient_ids",
                "snippet", "snippet_cs", "read", "error", "has_attachment"
        };

        StringBuilder dataBuilder = new StringBuilder();

        try {
            Uri uri = Uri.parse(CONVERSATION_URI.toString());
            Cursor cursor = m_activity.getContentResolver().query(uri, ALL_THREADS_PROJECTION, null, null, "date desc");

            if (cursor.moveToPosition(from)) {

                int cnter = 0;

                do {
                    long id = cursor.getLong(0);
                    long date = cursor.getLong(1);
                    long msgCount = cursor.getLong(2);
                    String recipIDs = cursor.getString(3);
                    String snippet = cursor.getString(4);
                    long snippetCS = cursor.getLong(5);
                    long read = cursor.getLong(6);
                    long error = cursor.getLong(7);
                    long hasAttach = cursor.getLong(8);

                    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                    Date d = new Date(date);
                    String strDate = dateFormat.format(d);

                    dataBuilder.append(id);
                    dataBuilder.append(",");
                    dataBuilder.append(strDate);
                    dataBuilder.append(",");
                    dataBuilder.append(msgCount);
                    dataBuilder.append(",");
                    dataBuilder.append(recipIDs);
                    dataBuilder.append(",");
                    dataBuilder.append(Base64Util.encodeString(snippet));
                    dataBuilder.append(",");
                    dataBuilder.append(snippetCS);
                    dataBuilder.append(",");
                    dataBuilder.append(read);
                    dataBuilder.append(",");
                    dataBuilder.append(error);
                    dataBuilder.append(",");
                    dataBuilder.append(hasAttach);
                    dataBuilder.append("\n");

                } while (++cnter < count && cursor.moveToNext());

            }
            else {
                Log.d("java > readConversionList: ", "cursor.moveToPosition(" + String.valueOf(from) + ") FAILED!");
            }

            if (!cursor.isClosed()) {
                cursor.close();
                cursor = null;
            }

        }
        catch (SQLiteException ex) {
            Log.d("SQLiteException in readConversionList", ex.getMessage());
        }

        return dataBuilder.toString();
    }

    static public int findConversionDataByAddress(String address)
    {
        return 0;
    }

    static public void updateMessageAsReaded(int threadid)
    {
        if (m_activity == null)
            return ;

        final String SMS_URI_ALL = "content://sms/inbox";

        StringBuilder dataBuilder = new StringBuilder();

        try {
            Uri uri = Uri.parse(SMS_URI_ALL);
            String[] projection = new String[] { "_id" };
            Cursor cursor = m_activity.getContentResolver().query(uri, projection, "thread_id = " + threadid + " AND read = 0", null, null);

            if (cursor.moveToFirst()) {
                int cnter = 0;
                do {
                    if (cnter > 0) dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(0));

                    cnter++;
                } while (cursor.moveToNext());

                if (!cursor.isClosed()) {
                    cursor.close();
                    cursor = null;
                }
            }

        } catch (SQLiteException ex) {
            Log.d("SQLiteException in updateMessageAsReaded", ex.getMessage());
        }

        ContentValues cv = new ContentValues();
        cv.put("read", "1");
        m_activity.getContentResolver().update(Uri.parse("content://sms"), cv, " thread_id=? and _id IN (?)", new String[]{ threadid+"", dataBuilder.toString() });
    }

    static public String readConversionData(int threadid, int from, int count)
    {
        if (m_activity == null)
            return "";

        final String SMS_URI_ALL = "content://sms/";

        StringBuilder dataBuilder = new StringBuilder();

        try {
            Uri uri = Uri.parse(SMS_URI_ALL);
            String[] projection = new String[] { "_id", "address", "person", "body",
                                                 "date", "type", "protocol", "read", "status" };
            Cursor cursor = m_activity.getContentResolver().query(uri, projection, "thread_id = " + threadid, null, "date desc");

            if (cursor.moveToPosition(from)) {

                int cnter = 0;

                do {
                    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                    Date d = new Date(cursor.getLong(4));
                    String strDate = dateFormat.format(d);

                    dataBuilder.append(cursor.getLong(0));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getString(1));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getString(2));
                    dataBuilder.append(",");
                    dataBuilder.append(Base64Util.encodeString(cursor.getString(3)));
                    dataBuilder.append(",");
                    dataBuilder.append(strDate);
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(5));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(6));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(7));
                    dataBuilder.append(",");
                    dataBuilder.append(cursor.getLong(8));
                    dataBuilder.append("\n");

                } while (++cnter < count && cursor.moveToNext());

                if (!cursor.isClosed()) {
                    cursor.close();
                    cursor = null;
                }
            }

        } catch (SQLiteException ex) {
            Log.d("SQLiteException in readConversionData", ex.getMessage());
        }

        return dataBuilder.toString();
    }

}
