package krypto.orders;

import krypto.serialization.OrderUpdate;

public interface OrderListener {
    void handleOrderEvent(final OrderUpdate orderUpdate);
}
