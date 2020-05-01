package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableModel;
import java.util.Map;

public class ParametersTable extends JTable {
    private Map<Integer, TableCellEditor> editors;

    public ParametersTable(TableModel model, Map<Integer, TableCellEditor> editors) {
        super(model);
        this.editors = editors;
    }

    @Override
    public TableCellEditor getCellEditor(int row, int column) {
        if (editors.containsKey(row))
            return editors.get(row);
        else
            return super.getCellEditor(row, column);
    }
}
