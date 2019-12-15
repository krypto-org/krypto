package krypto.ui.instruments;

import krypto.ui.UIDataCache;
import krypto.ui.components.FontConstants;
import krypto.ui.components.TableColumnHeaderRenderer;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.swing.table.TableRowSorter;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

public class InstrumentsView extends JFrame {
    private final JPanel contentPane;
    private final JTextField txtSearch = new JTextField();
    private final JLabel lblSearch = new JLabel("SEARCH");

    private final JScrollPane instrumentsTableSP = new JScrollPane();
    private final InstrumentsTableModel instrumentsTableModel;
    private final InstrumentsTable instrumentsTable;
//    private final TableRowSorter<InstrumentsTableModel> instrumentsSorter;

    public InstrumentsView(final UIDataCache dataCache)
    {
        this.setTitle("INSTRUMENTS");
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setBounds(100, 100, 800, 500);
        contentPane = new JPanel();
        contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
        setContentPane(contentPane);
        contentPane.setLayout(
                new MigLayout("",
                        "[100px,fill][grow]",
                        "[30px,fill][grow]"));

        this.instrumentsTableModel = new InstrumentsTableModel(dataCache);
        this.instrumentsTable = new InstrumentsTable(instrumentsTableModel);
//        this.instrumentsSorter = new TableRowSorter<>();

        this.initializeInstrumentsTable();
        this.initializeSearchText();
    }

    private void initializeSearchText()
    {
        lblSearch.setFont(FontConstants.LABEL_FONT);
        contentPane.add(lblSearch, "cell 0 0,alignx trailing,hmin 30");
//        instrumentsTable.setRowSorter(instrumentsSorter);
        txtSearch.setFont(FontConstants.TEXT_BOX_FONT);
        txtSearch.addKeyListener(new KeyAdapter()
        {
            @Override
            public void keyReleased(KeyEvent e)
            {
                String text = txtSearch.getText().toUpperCase();
                if (text.length() == 0)
                {
//                    instrumentsSorter.setRowFilter(null);
                }
                else
                {
//                    instrumentsSorter.setRowFilter(RowFilter.regexFilter(text));
                }
            }
        });
        contentPane.add(txtSearch, "cell 1 0,growx,hmin 30");
        txtSearch.setColumns(10);
    }

    private void initializeInstrumentsTable()
    {
        contentPane.add(instrumentsTableSP, "cell 0 1 2 1,grow");

        instrumentsTable.setFont(FontConstants.TABLE_TEXT_FONT);
        instrumentsTableSP.setViewportView(instrumentsTable);
        instrumentsTable.getTableHeader()
                .setDefaultRenderer(new TableColumnHeaderRenderer());
    }

    public void refresh() {
        this.instrumentsTableModel.reload();
    }

}
