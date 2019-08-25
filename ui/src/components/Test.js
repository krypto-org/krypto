import React from "react";
import { connect } from "react-redux";

class Test extends React.Component {
  render() {
    return (
      <div>
        Test
      </div>
    )
  }
}

export default connect((store) => store)(Test);
