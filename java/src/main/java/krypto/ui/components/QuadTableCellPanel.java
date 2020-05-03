package krypto.ui.components;

import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import javax.swing.border.LineBorder;
import java.awt.*;

public class QuadTableCellPanel extends JPanel {
    private JLabel lblLeft;
    private JLabel lblRight;
    private JLabel lblMidTop;
    private JLabel lblMidBottom;

    public QuadTableCellPanel() {
        super();
        setBorder(new LineBorder(Color.GRAY, 2));
        setLayout(new MigLayout("ins 0, wrap 1",
                "[55px,fill][50px,grow,fill][55px,fill]",
                "[grow,fill][grow,fill]"));

        this.setOpaque(false);

        lblLeft = new JLabel();
        lblLeft.setHorizontalAlignment(SwingConstants.CENTER);
        lblLeft.setFont(FontConstants.LABEL_FONT);
        lblLeft.setBackground(ColorConstants.CALL_COLOR);
        lblLeft.setOpaque(true);
        add(lblLeft, "cell 0 0 1 2");

        lblMidTop = new JLabel();
        lblMidTop.setHorizontalAlignment(SwingConstants.CENTER);
        lblMidTop.setFont(FontConstants.LABEL_FONT_TINY);
        lblMidTop.setBackground(ColorConstants.CALL_COLOR);
        lblMidTop.setOpaque(true);
        add(lblMidTop, "cell 1 0,growx,aligny bottom");

        lblRight = new JLabel();
        lblRight.setFont(FontConstants.LABEL_FONT);
        lblRight.setHorizontalAlignment(SwingConstants.CENTER);
        lblRight.setBackground(ColorConstants.PUT_COLOR);
        lblRight.setOpaque(true);
        add(lblRight, "cell 2 0 1 2");

        lblMidBottom = new JLabel();
        lblMidBottom.setHorizontalAlignment(SwingConstants.CENTER);
        lblMidBottom.setFont(FontConstants.LABEL_FONT_TINY);
        lblMidBottom.setBackground(ColorConstants.PUT_COLOR);
        lblMidBottom.setOpaque(true);
        add(lblMidBottom, "cell 1 1,growx,aligny top");
    }

    public JLabel getLeftLabel() {
        return lblLeft;
    }

    public JLabel getRightLabel() {
        return lblRight;
    }

    public JLabel getMiddleTopLabel() {
        return lblMidTop;
    }

    public JLabel getMiddleBottomLabel() {
        return lblMidBottom;
    }
}
