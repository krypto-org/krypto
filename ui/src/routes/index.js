/* eslint-disable react/display-name */
import React from "react";
import Prices from "../components/Prices/Prices";
import Instruments from "../components/Instruments/Instruments";

const routes = [
  {
    path: "/",
    exact: true,
    sidebar: "Prices",
    icon: "stock",
    comp: Prices
  },
  {
    path: "/instruments",
    sidebar: "Instruments",
    icon: "database",
    comp: Instruments
  },
  {
    path: "/orders",
    sidebar: "Orders",
    icon: "transaction",
    comp: () => <div>Orders</div>
  },
];

export default routes;
