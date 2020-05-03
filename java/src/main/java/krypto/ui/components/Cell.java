package krypto.ui.components;

public class Cell
{
	private final int row;
	private final int column;

	public Cell(int row, int column)
	{
		this.row = row;
		this.column = column;
	}

	public boolean is(int r, int c)
	{
		return row == r && column == c;
	}

	public int getRow()
	{
		return row;
	}

	public int getColumn()
	{
		return column;
	}
}
