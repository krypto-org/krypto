package krypto.testapps;

import krypto.instruments.InstrumentsClient;
import krypto.serialization.Exchange;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.zeromq.ZMQ;

public class InstrumentClientApp {
  private static final Logger logger = LogManager.getLogger(InstrumentClientApp.class);

  public static void main(String[] args) {
    final ZMQ.Context context = ZMQ.context(1);
    final InstrumentsClient instrumentsClient =
        new InstrumentsClient(context, "tcp://localhost:12000");
    instrumentsClient
        .getInstruments()
        .forEach(
            (id, inst) -> {
              logger.info("{}: {}@{}", id, inst.symbol(), Exchange.name(inst.exchange()));
            });
  }
}
