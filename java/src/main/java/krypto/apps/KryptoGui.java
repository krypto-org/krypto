package krypto.apps;

import com.typesafe.config.Config;
import com.typesafe.config.ConfigFactory;
import krypto.instruments.InstrumentsClient;
import krypto.mktdata.Subscriber;
import krypto.ui.StartScreen;
import krypto.ui.UIDataCache;
import org.zeromq.ZMQ;

import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.util.Collections;

public class KryptoGui {
    public static void main(String[] args) {
        final Config config = ConfigFactory.parseFile(new File("config.hocon")).resolve();
        final ZMQ.Context context = ZMQ.context(1);

        final InstrumentsClient instrumentsClient = new InstrumentsClient(context, "tcp://192.168.1.12:12000");

        Subscriber subscriber = new Subscriber(
                context,
                Collections.singletonList("tcp://192.168.1.12:12021"),
                true);
        subscriber.subscribe("");
        subscriber.start();

        final UIDataCache uiDataCache = new UIDataCache(instrumentsClient);
        subscriber.registerListener(uiDataCache);

        EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {
                try {
                    UIManager.setLookAndFeel(
                            "com.jtattoo.plaf.noire.NoireLookAndFeel");
                    UIManager.put("InternalFrame.titleFont",
                            new Font("Cambria Math", Font.BOLD, 12));
                    StartScreen frame = new StartScreen(uiDataCache);
                    frame.setVisible(true);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
    }
}
