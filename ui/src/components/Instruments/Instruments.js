import React, { Component } from "react";
import styles from "./Instruments.scss"
import { connect } from 'react-redux';
import ReactTable from "react-table";
import PropTypes from "prop-types"
import "react-table/react-table.css";

class Instruments extends Component {
  render() {
    return (
      <div className={styles.instruments}>
        <ReactTable
          data={this.props.instruments}
          columns={[
            {
              Header: "Id",
              accessor: "id"
            },
            {
              Header: "Type",
              accessor: "type"
            },
            {
              Header: "Symbol",
              accessor: "symbol"
            },
            {
              Header: "Exchange",
              accessor: "exchange"
            },
            {
              Header: "Exchange Symbol",
              accessor: "exchangeSymbol"
            },
            {
              Header: "Tick Size",
              accessor: "tickSize"
            },
            {
              Header: "Min Size",
              accessor: "minSize"
            },
            {
              Header: "Max Size",
              accessor: "maxSize"
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
        />
      </div>);
  }
}

Instruments.propTypes = {
  instruments: PropTypes.array.isRequired
}

const mapStateToProps = (state, ownProps) => {
  return {
    ...ownProps,
    instruments: state.instruments.instruments
  }
}

export default connect(mapStateToProps)(Instruments);
