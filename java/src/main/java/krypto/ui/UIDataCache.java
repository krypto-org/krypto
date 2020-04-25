package krypto.ui;

import krypto.instruments.InstrumentsClient;
import krypto.mktdata.Subscriber;
import krypto.pricing.PricingClient;
import krypto.serialization.*;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.stream.Collectors;

public class UIDataCache implements
        Subscriber.Listener, PricingClient.Listener {

    private final InstrumentsClient instrumentsClient;
    private final Map<Long, Quote> quotes;
    private final Map<Long, TheoreticalSnapshot> theos;
    private final SortedMap<Long, Instrument> instruments;
    private final Set<Long> activeInstruments;

    public UIDataCache(final InstrumentsClient instrumentsClient) {
        this.instrumentsClient = instrumentsClient;
        this.instruments = new TreeMap<>();
        this.quotes = new ConcurrentHashMap<>();
        this.theos = new ConcurrentHashMap<>();
        this.activeInstruments = new HashSet<>();
    }

    @Override
    public void onHeartbeat(final Heartbeat hb) {

    }

    @Override
    public void onQuote(final Quote quote) {
        if (this.activeInstruments.contains(quote.securityId())) {
            this.quotes.put(quote.securityId(), quote);
        }
    }

    @Override
    public void onTrade(final Trade quote) {

    }

    @Override
    public void onOrderUpdate(final OrderUpdate ou) {

    }

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
            this.instruments.values().forEach(inst -> {
                if (inst.active()) {
                    activeInstruments.add(inst.id());
                }
            });
        }
        return instruments;
    }

    public Map<Long, Instrument> getActiveInstruments(final boolean reloadCache) {
        return this.getInstruments(reloadCache).values().stream().filter(
                Instrument::active).collect(
                        Collectors.toMap(Instrument::id, instrument -> instrument));
    }

    public Map<Long, Quote> getQuotes() {
        return quotes;
    }

    public Map<Long, TheoreticalSnapshot> getTheos() {
        return theos;
    }
}
