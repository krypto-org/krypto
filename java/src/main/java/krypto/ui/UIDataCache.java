package krypto.ui;

import krypto.instruments.InstrumentsClient;
import krypto.mktdata.Conversion;
import krypto.mktdata.Subscriber;
import krypto.orders.Fill;
import krypto.orders.Order;
import krypto.orders.OrderListener;
import krypto.pricing.PricingClient;
import krypto.serialization.*;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;
import java.util.stream.Collectors;

public class UIDataCache implements Subscriber.Listener, PricingClient.Listener, OrderListener {

    private final InstrumentsClient instrumentsClient;
    private final Map<Long, Quote> quotes;
    private final Map<Long, TheoreticalSnapshot> theos;
    private final Map<String, Order> orders;
    private final List<Fill> fills;
    private final SortedMap<Long, Instrument> instruments;
    private final Map<String, Long> symbolToInstrumentIdMapping;
    private final Set<Long> activeInstruments;

    public UIDataCache(final InstrumentsClient instrumentsClient) {
        this.instrumentsClient = instrumentsClient;
        this.instruments = new TreeMap<>();
        this.quotes = new ConcurrentHashMap<>();
        this.theos = new ConcurrentHashMap<>();
        this.orders = new ConcurrentHashMap<>();
        this.fills = new ArrayList<>();
        this.symbolToInstrumentIdMapping = new ConcurrentHashMap<>();
        this.activeInstruments = new ConcurrentSkipListSet<>();
    }

    @Override
    public void onHeartbeat(final Heartbeat hb) {}

    @Override
    public void onQuote(final Quote quote) {
        if (this.activeInstruments.contains(quote.securityId())) {
            this.quotes.put(quote.securityId(), quote);
        }
    }

    @Override
    public void onTrade(final Trade quote) {}

    @Override
    public void onOrderUpdate(final OrderUpdate ou) {}

    @Override
    public void onTheoreticalSnapshot(TheoreticalSnapshot snapshot) {
        if (this.activeInstruments.contains(snapshot.securityId())) {
            this.theos.put(snapshot.securityId(), snapshot);
        }
    }

    public SortedMap<Long, Instrument> getInstruments(final boolean reloadCache) {
        if (reloadCache || this.instruments.isEmpty()) {
            this.instruments.clear();
            this.activeInstruments.clear();
            instrumentsClient.getInstruments().forEach(this.instruments::put);
            this.instruments
                    .values()
                    .forEach(
                            inst -> {
                                if (inst.active()) {
                                    activeInstruments.add(inst.id());
                                    symbolToInstrumentIdMapping.put(inst.symbol(), inst.id());
                                }
                            });
        }
        return instruments;
    }

    public Map<Long, Instrument> getActiveInstruments(final boolean reloadCache) {
        return this.getInstruments(reloadCache).values().stream()
                .filter(Instrument::active)
                .collect(Collectors.toMap(Instrument::id, instrument -> instrument));
    }

    public Map<String, Long> getSymbolToInstrumentIdMapping() {
        return symbolToInstrumentIdMapping;
    }

    public Map<Long, Quote> getQuotes() {
        return quotes;
    }

    public Map<Long, TheoreticalSnapshot> getTheos() {
        return theos;
    }

    public Map<String, Order> getOrders() {
        return orders;
    }

    public List<Fill> getFills() {
        return fills;
    }

    @Override
    public void handleOrderEvent(final OrderUpdate orderUpdate) {
        if (this.orders.containsKey(orderUpdate.orderId())) {
            final Order order = this.orders.get(orderUpdate.orderId());
            final double filledQty = Conversion.convertQuantity(orderUpdate.filledQuantity());
            order.setOrderId(orderUpdate.exchangeOrderId());
            order.setStatus(orderUpdate.status());
            order.setFilledQty(filledQty);
            order.setFees(0.005 * filledQty * order.getPrice());

            if (orderUpdate.status() == OrderStatus.FILLED
                    || orderUpdate.status() == OrderStatus.PARTIALLY_FILLED) {
                final Fill fill =
                        new Fill(
                                orderUpdate.exchangeOrderId(),
                                order.getSide(),
                                order.getPrice(),
                                order.getFilledQty(),
                                order.getSize() - order.getFilledQty());
                this.fills.add(fill);
            }
        }
    }

    public void registerOrder(final Order order) {
        this.orders.put(order.getClOrderId(), order);
    }
}
