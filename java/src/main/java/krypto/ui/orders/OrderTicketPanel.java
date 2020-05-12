package krypto.ui.orders;

import krypto.orders.Order;
import krypto.orders.OrderClient;
import krypto.serialization.Quote;
import krypto.serialization.Side;
import krypto.serialization.TimeInForce;
import krypto.ui.UIDataCache;
import net.miginfocom.swing.MigLayout;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import javax.swing.*;

public class OrderTicketPanel extends JPanel {

    private static final Logger logger = LogManager.getLogger(OrderTicketPanel.class);

    private static final String EXCHANGE = "COINBASE_SANDBOX";

    private final OrderTicketTableModel tableModel;
    private final OrderClient client;
    private final UIDataCache uiDataCache;

    public OrderTicketPanel(final OrderClient client, final UIDataCache uiDataCache) {
        this.client = client;
        this.uiDataCache = uiDataCache;
        this.setLayout(
                new MigLayout("",
                        "[fill,grow][100,fill]",
                        "[fill,grow]"));
        this.tableModel = new OrderTicketTableModel();
        OrderTicketTable orderTicketTable = new OrderTicketTable(this.tableModel);
        final JScrollPane scrollPane = new JScrollPane();
        scrollPane.setViewportView(orderTicketTable);

        JButton sendButton = new JButton("Send");
        sendButton.addActionListener(e -> {
            this.sendOrder();
        });

        this.add(scrollPane );
        this.add(sendButton);
    }

    public void setSelectedInstrument(final long id) {
        final Quote quote = this.uiDataCache.getQuotes().get(id);
        final String symbol = this.uiDataCache
                .getActiveInstruments(false)
                .get(id).symbol();
        this.tableModel.update(symbol, quote);
    }

    private void sendOrder() {
        final String symbol = this.tableModel.getSymbol();
        if (!this.uiDataCache.getSymbolToInstrumentIdMapping().containsKey(symbol)) {
            logger.error("Cannot find id for symbol {}", symbol);
        }
        final long id = this.uiDataCache.getSymbolToInstrumentIdMapping().get(symbol);
        final double price = this.tableModel.getPrice();
        final double qty = this.tableModel.getSize();
        final byte side = this.tableModel.getSide().getValue();
        final byte tif = this.tableModel.getTif().getValue();
        final String exchange = this.tableModel.getExchange().toString();
        logger.info("Sending order for symbol {} [{}] {} {}@{} | {}",
                symbol, id, Side.name(side), qty, price, TimeInForce.name(tif));
        final var orderId = this.client.newOrder(exchange, id, price, qty, side, tif);
        this.uiDataCache.registerOrder(new Order(orderId, id, symbol,price, qty, side, tif));
    }
}
