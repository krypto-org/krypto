package krypto.apps;

import com.github.weisj.darklaf.LafManager;
import com.github.weisj.darklaf.theme.DarculaTheme;
import krypto.Config;
import krypto.instruments.InstrumentsClient;
import krypto.mktdata.MktdataSubscriber;
import krypto.orders.OrderClient;
import krypto.orders.OrderSubscriber;
import krypto.ui.StartScreen;
import krypto.ui.UIDataCache;
import org.zeromq.ZMQ;

import java.awt.*;
import java.io.IOException;
import java.util.Collections;

public class KryptoGui {
  public static void main(String[] args) throws IOException {
    final var config = new Config(args[0]);

    final var context = ZMQ.context(3);

    final var instrumentsClient =
        new InstrumentsClient(context, config.getStringNode("/services/instruments/client"));

    final var mktdataSubscriber =
        new MktdataSubscriber(
            context,
            Collections.singletonList(
                config.getStringNode("/services/mktdata_gateway/backend/client")),
            true);
    mktdataSubscriber.subscribe("");


    final var orderSubscriber =
            new OrderSubscriber(
                    context,
                    Collections.singletonList(
                            config.getStringNode("/services/order_gateway/broadcast/client")));
    orderSubscriber.subscribe("");

    final var uiDataCache = new UIDataCache(instrumentsClient);
    mktdataSubscriber.registerListener(uiDataCache);
    orderSubscriber.registerListener(uiDataCache);

    final OrderClient orderClient =
        new OrderClient(
            context, config.getStringNode("/services/order_gateway/frontend/client"), 100);

    mktdataSubscriber.start();
    orderClient.start();
    orderSubscriber.start();

    EventQueue.invokeLater(
        () -> {
          try {
            LafManager.install(new DarculaTheme());
            final var frame = new StartScreen(uiDataCache, orderClient);
            frame.setVisible(true);
          } catch (Exception e) {
            e.printStackTrace();
          }
        });
  }
}
