package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableModel;
import java.awt.*;
import java.util.List;
import java.util.Objects;
import java.util.function.Function;

public class ComboBoxTableCellEditor<T> extends DefaultCellEditor {
    public ComboBoxTableCellEditor(final T[] values) {
        super(new JComboBox<>(values));
    }
}
