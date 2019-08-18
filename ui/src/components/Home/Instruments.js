import React, { Component } from "react";
import { flatbuffers } from "flatbuffers";
import krypto from "../../krypto/serializaton_generated";
import zeromq from "zeromq"
import _ from "lodash"

import ReactTable from "react-table";
import "react-table/react-table.css";

class Instruments extends Component {
  constructor(props) {
    super(props);
    this.state = {
      data: []
    };
    this.requester = zeromq.socket('req')
    this.requester.identity = "ui-" + Date.now()
    this.requester.connect("tcp://127.0.0.1:8686");
    const self = this
    this.requester.on('message', function (service, payload) {
      console.log('got reply from ', service.toString(), ":", payload);
      self.parseInstruments(payload).then((value) => {
        self.setState({ ...self.state, data: value }, console.log)
      }).then(() => {
        console.log(self.state.data)
      })
    });
  }

  componentDidMount() {
    const req = this.createRequest();
    console.log(req.toString());
    this.requester.send(["instruments", req.toString()])
  }

  parseInstruments = async (payload) => {
    const bb = new flatbuffers.ByteBuffer(payload);
    const response = krypto.serialization.InstrumentResponse.getRootAsInstrumentResponse(bb)
    const instrLen = response.instrumentsLength()
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
    })
  }

  createRequest = () => {
    var builder = new flatbuffers.Builder(1024);
    krypto.serialization.InstrumentRequest.startInstrumentRequest(builder);
    krypto.serialization.InstrumentRequest.addType(builder, krypto.serialization.RequestType.ALL);
    const instrumentRequest = krypto.serialization.InstrumentRequest.endInstrumentRequest(builder);
    builder.finish(instrumentRequest);
    const buf = builder.asUint8Array();
    return buf;
  }

  render() {
    return <ReactTable
      data={this.state.data}
      columns={[
        {
          Header: "Id",
          accessor: "id",
          maxWidth: 200
        },
        {
          Header: "Type",
          accessor: "type",
          maxWidth: 200
        },
        {
          Header: "Symbol",
          accessor: "symbol",
          maxWidth: 200
        },
        {
          Header: "Exchange",
          accessor: "exchange",
          maxWidth: 200
        },
        {
          Header: "Exchange Symbol",
          accessor: "exchangeSymbol",
          maxWidth: 200
        },
        {
          Header: "Tick Size",
          accessor: "tickSize",
          maxWidth: 200
        },
        {
          Header: "Min Size",
          accessor: "minSize",
          maxWidth: 200
        },
        {
          Header: "Max Size",
          accessor: "maxSize",
          maxWidth: 200
        }
      ]}
      defaultPageSize={25}
      className="-striped -highlight"
      getTdProps={() => ({
        style: {
          textAlign: 'center'
        }
      })}
      style={{
        height: "800px",
        width: "100%"
      }}
    />;
  }
}

export default Instruments;
