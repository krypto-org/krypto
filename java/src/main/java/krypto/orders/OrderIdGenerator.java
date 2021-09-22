package krypto.orders;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

public class OrderIdGenerator {

  private long baseId;

  public OrderIdGenerator(final int size) {
    this.baseId = this.generateBaseId(size);
  }

  public String generate() {
    return String.format("%d", ++baseId);
  }

  private long generateBaseId(final long size) {
    if (size < 7) {
      throw new IllegalArgumentException("Size should be greater than 6");
    }
    final long multiplier = (long) Math.pow(10.0, size - 6);
    final var date = Calendar.getInstance().getTime();
    final var formatter = new SimpleDateFormat("HHmmss");
    final var dateStr = formatter.format(date);
    return Integer.parseInt(dateStr) * multiplier;
  }
}
