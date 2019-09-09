import zeromq from "zeromq"
import { brokerIpAddress } from "../krypto"

export const requesterSocket = () => {
  const sock = zeromq.socket('req');
  sock.identity = "ui-" + Date.now();
  sock.connect(`tcp://${brokerIpAddress}:8686`);
  return sock;
}
