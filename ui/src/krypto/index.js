import { flatbuffers } from "flatbuffers";
import zeromq from "zeromq"
import _ from "lodash"
import krypto from "./serialization_generated";

export const createInstrumentRequest = () => {
  var builder = new flatbuffers.Builder(1024);
  krypto.serialization.InstrumentRequest.startInstrumentRequest(builder);
  krypto.serialization.InstrumentRequest.addType(builder, krypto.serialization.RequestType.ALL);
  const instrumentRequest = krypto.serialization.InstrumentRequest.endInstrumentRequest(builder);
  builder.finish(instrumentRequest);
  const buf = builder.asUint8Array();
  return buf;
}

export const requesterSocket = () => {
  const sock = zeromq.socket('req');
  sock.identity = "ui-" + Date.now();
  sock.connect("tcp://192.168.1.6:8686");
  return sock;
}

export const parseInstruments = async (payload) => {
  const bb = new flatbuffers.ByteBuffer(payload);
  const response = krypto.serialization.InstrumentResponse.getRootAsInstrumentResponse(bb);
  const instrLen = response.instrumentsLength();
  return _.range(instrLen).map((val) => {
    const inst = response.instruments(val)

    return {
      id: inst.id().toFloat64().toString(),
      type: inst.type(),
      symbol: inst.symbol(),
      exchange: inst.exchange(),
      exchangeSymbol: inst.exchangeSymbol(),
      tickSize: inst.tickSize(),
      minSize: inst.maxSize(),
      maxSize: inst.maxSize(),
      cryptoBase: inst.cryptoBase(),
      cryptoQuote: inst.cryptoQuote()
    }
  });
}
