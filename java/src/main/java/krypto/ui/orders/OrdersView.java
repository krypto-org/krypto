package krypto.ui.orders;

import krypto.ui.UIDataCache;
import krypto.ui.components.LiveFrame;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import javax.swing.border.EmptyBorder;

public class OrdersView extends LiveFrame {

    private final UIDataCache uiDataCache;
    private final FillsTable fillsTable;
    private final FillsTableModel fillsTableModel;
    private final OrdersTable ordersTable;
    private final OrdersTableModel ordersTableModel;

    public OrdersView(final UIDataCache uiDataCache) {
        this.setTitle("ORDERS");
        this.uiDataCache = uiDataCache;
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setBounds(100, 100, 800, 500);
        JPanel contentPane = new JPanel();

        contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
        setContentPane(contentPane);
        contentPane.setLayout(
                new MigLayout("",
                        "[fill,grow]",
                        "[fill,grow]"));

        final var splitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
        final var ordersTableScrollPane = new JScrollPane();
        this.ordersTableModel = new OrdersTableModel();
        this.ordersTable = new OrdersTable(this.ordersTableModel);

        final var fillssTableScrollPane = new JScrollPane();
        this.fillsTableModel = new FillsTableModel();
        this.fillsTable = new FillsTable(this.fillsTableModel);

        splitPane.setLeftComponent(ordersTableScrollPane);
        splitPane.setRightComponent(fillssTableScrollPane);
        splitPane.setDividerLocation(0.5);
        splitPane.setResizeWeight(0.5);
        contentPane.add(splitPane, "cell 0 0 1 1");
    }

    @Override
    protected void refreshUi() {

    }
}
