package krypto.ui.components;

public class Cell
{
	private int row;
	private int column;

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
