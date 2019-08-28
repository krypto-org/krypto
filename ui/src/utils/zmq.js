import zeromq from "zeromq"

export const requesterSocket = () => {
  const sock = zeromq.socket('req');
  sock.identity = "ui-" + Date.now();
  sock.connect("tcp://192.168.1.6:8686");
  return sock;
}
