package krypto;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.File;
import java.io.IOException;

public class Config {
  private final JsonNode root;

  public Config(final String configFile) throws IOException {
    this(new ObjectMapper().readTree(new File(configFile)));
  }

  public Config(JsonNode root) {
    this.root = root;
  }

  public JsonNode getRoot() {
    return root;
  }

  public JsonNode getNode(final String path) {
    return root.at(path);
  }

  public int getIntNode(final String path) {
    return getNode(path).asInt();
  }

  public double getDoubleNode(final String path) {
    return getNode(path).asDouble();
  }

  public long getLongNode(final String path) {
    return getNode(path).asLong();
  }

  public boolean getBooleanNode(final String path) {
    return getNode(path).asBoolean();
  }

  public String getStringNode(final String path) {
    return getNode(path).asText();
  }
}
