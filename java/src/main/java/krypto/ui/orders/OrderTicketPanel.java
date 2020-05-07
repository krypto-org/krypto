package krypto.ui.orders;

import krypto.serialization.Quote;
import krypto.ui.UIDataCache;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;

public class OrderTicketPanel extends JPanel {
    private final OrderTicketTableModel tableModel;
    private final UIDataCache uiDataCache;

    public OrderTicketPanel(final UIDataCache uiDataCache) {
        this.uiDataCache = uiDataCache;
        this.setLayout(
                new MigLayout("",
                        "[fill,grow]",
                        "[fill,grow]"));
        this.tableModel = new OrderTicketTableModel();
        OrderTicketTable orderTicketTable = new OrderTicketTable(this.tableModel);
        this.add(orderTicketTable, "cell 0 0 1 1");
    }

    public void setSelectedInstrument(final long id) {
        final Quote quote = this.uiDataCache.getQuotes().get(id);
        final String symbol = this.uiDataCache
                .getActiveInstruments(false)
                .get(id).symbol();
        this.tableModel.update(symbol, quote);
    }
}
