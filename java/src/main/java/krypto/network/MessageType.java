package krypto.network;

import org.apache.logging.log4j.core.config.Order;

import java.util.HashMap;
import java.util.Map;

public class MessageType {
    public static final byte UNDEFINED = 0;
    public static final byte NO_PAYLOAD = 1;
    public static final byte QUOTE = 2;
    public static final byte TRADE = 3;
    public static final byte HEARTBEAT = 4;
    public static final byte INSTRUMENT_REQUEST = 5;
    public static final byte INSTRUMENT_RESPONSE = 6;
    public static final byte INSTRUMENT_CACHE_REFRESH_REQUEST = 7;
    public static final byte ORDER = 8;
    public static final byte ORDER_REQUEST = 9;
    public static final byte ORDER_CANCEL_REQUEST = 10;
    public static final byte ORDER_REPLACE_REQUEST = 11;
    public static final byte ORDER_UPDATE = 12;
    public static final byte POSITION = 13;
    public static final byte RISK_SUMMARY = 14;

    public static final Map<String, Byte> MsgTypeNames = new HashMap<>();

    static {
        MsgTypeNames.put("", UNDEFINED);
        MsgTypeNames.put("AA", NO_PAYLOAD);
        MsgTypeNames.put("MQ", QUOTE);
        MsgTypeNames.put("MT", TRADE);
        MsgTypeNames.put("HH", HEARTBEAT);
        MsgTypeNames.put("IQ", INSTRUMENT_REQUEST);
        MsgTypeNames.put("IS", INSTRUMENT_RESPONSE);
        MsgTypeNames.put("IC", INSTRUMENT_CACHE_REFRESH_REQUEST);
        MsgTypeNames.put("O_", ORDER);
        MsgTypeNames.put("OO", ORDER_REQUEST);
        MsgTypeNames.put("OC", ORDER_CANCEL_REQUEST);
        MsgTypeNames.put("OR", ORDER_REPLACE_REQUEST);
        MsgTypeNames.put("OU", ORDER_UPDATE);
        MsgTypeNames.put("PP", POSITION);
        MsgTypeNames.put("PR", RISK_SUMMARY);

    }
}
