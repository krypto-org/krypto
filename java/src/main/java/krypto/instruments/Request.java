package krypto.instruments;

import krypto.serialization.RequestType;

public class Request {
  private final byte requestType;

  public Request(final byte requestType) {
    this.requestType = requestType;
  }

  public Request() {
    this(RequestType.ALL);
  }

  public byte getRequestType() {
    return requestType;
  }
}
