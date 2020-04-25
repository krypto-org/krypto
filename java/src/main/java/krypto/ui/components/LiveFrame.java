package krypto.ui.components;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Timer;
import java.util.TimerTask;

public abstract class LiveFrame extends JFrame
{
	protected Timer updateTimer;
	protected int updateFrequency = 250;
	protected JButton btnManualRefresh;
	private boolean isIdle = true;
	private boolean timerRunning = false;

	public LiveFrame()
	{
		super();
		btnManualRefresh = new JButton("MANUAL REFRESH");
		btnManualRefresh.addActionListener(e -> refreshUi());
		btnManualRefresh.setFont(new Font("Cambria Math", Font.BOLD, 11));
	}

	public void startUpdates()
	{
		if (!timerRunning)
		{
			updateTimer = new Timer();
			updateTimer.schedule(new TimerTask()
			{

				@Override
				public void run()
				{
					try
					{
						if (isIdle)
						{
							isIdle = false;
							refreshUi();
							isIdle = true;
						}
					}
					catch (Exception ex)
					{
						isIdle = true;
					}
				}
			}, updateFrequency, updateFrequency);
			timerRunning = true;
		}
	}

	public void stopUpdates()
	{
		if (timerRunning)
		{
			updateTimer.cancel();
			updateTimer = null;
			isIdle = true;
			timerRunning = false;
		}
	}

	public int getUpdateFrequency()
	{
		return updateFrequency;
	}

	public void setUpdateFrequency(int updateFrequency)
	{
		this.updateFrequency = updateFrequency;
	}

	protected abstract void refreshUi();
}
