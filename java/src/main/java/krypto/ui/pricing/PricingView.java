package krypto.ui.pricing;


import krypto.ui.UIDataCache;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;

public class PricingView extends JFrame {
    private final JPanel contentPane;

    private final JScrollPane instrumentsTableSP = new JScrollPane();
    private final TheoreticalSnapshotTable theoreticalSnapshotTable;
    private final TheoreticalSnapshotTable instrumentsTable;

    public PricingView(final UIDataCache uiDataCache)  {
        this.setTitle("PRICING");
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setBounds(100, 100, 800, 500);
        contentPane = new JPanel();
        contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
        setContentPane(contentPane);
        contentPane.setLayout(
                new MigLayout("",
                        "[fill][grow]",
                        "[fill][grow]"));

    }
}
