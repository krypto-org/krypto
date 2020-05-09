package krypto.ui.orders;

import krypto.orders.OrderListener;
import krypto.serialization.OrderStatus;
import krypto.serialization.OrderUpdate;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class UIOrderClientListener implements OrderListener {
    private static final Logger logger = LogManager.getLogger(UIOrderClientListener.class);

    public UIOrderClientListener() {}

    @Override
    public void handleOrderEvent(OrderUpdate orderUpdate) {
        logger.info("{} {}", orderUpdate.orderId(),
                OrderStatus.name(orderUpdate.status()));
    }
}
