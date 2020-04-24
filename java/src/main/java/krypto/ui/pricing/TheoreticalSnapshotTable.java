package krypto.ui.pricing;

import javax.swing.*;

public class TheoreticalSnapshotTable extends JTable {

    public enum Column {
        INSTRUMENT,
        BID_QUANTITY,
        MM_BID,
        THEO,
        ADJUSTED_THEO,
        ERROR,
        MM_ASK,
        ASK_QUANTITY,
        WIDTH_MULT
    }
}
