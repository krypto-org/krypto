package krypto.ui.orders;

import krypto.serialization.Quote;
import krypto.ui.UIDataCache;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;

public class OrderTicketPanel extends JPanel {
    private final OrderTicketTableModel tableModel;
    private final UIDataCache uiDataCache;
    private final JButton sendButton;

    public OrderTicketPanel(final UIDataCache uiDataCache) {
        this.uiDataCache = uiDataCache;
        this.setLayout(
                new MigLayout("",
                        "[fill,grow][100,fill]",
                        "[fill,grow]"));
        this.tableModel = new OrderTicketTableModel();
        OrderTicketTable orderTicketTable = new OrderTicketTable(this.tableModel);
        final JScrollPane scrollPane = new JScrollPane();
        scrollPane.setViewportView(orderTicketTable);

        sendButton = new JButton("Send");

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
}
