#include "peer.hh"
#include "../core/color.hh"

namespace Network
{

  Peer::Peer(const std::string& ip, in_port_t port)
    : ip_(ip)
    , port_(port)
    , fd_(0)
  { }

  void Peer::tryHandshake()
  {
    struct handshake hshake;

    std::string info_hash = torrent_->getInfoHash();
    std::string peer_id = torrent_->getPeerId();
    std::string protocol = "BitTorrent protocol";

    bzero(&hshake, sizeof (hshake));
    hshake.pstrlen = 19;

    memcpy(hshake.pstr, protocol.c_str(), 19);
    memcpy(hshake.info_hash, info_hash.c_str(), 20);
    memcpy(hshake.peer_id, peer_id.c_str(), 20);

    // FIXME: RETURN SOMETHING TO LET THE PD REMOVE PEER
    if (write(fd_, &hshake, sizeof (hshake)) < 0)
      std::cerr << "Could not write request to peer \n";

  }

  void Peer::dump() const
  {
    Core::ColorModifier clgreen(Core::ColorCode::FG_LIGHT_BLUE);
    Core::ColorModifier clblue(Core::ColorCode::FG_BLUE);
    Core::ColorModifier cdef(Core::ColorCode::FG_DEFAULT);

    std::cout << clblue << ip_  << " " << cdef << " "
	      << clgreen << port_ << cdef << std::endl;
  }

  // Getter
  std::string Peer::getIp() const
  {
    return ip_;
  }

  in_port_t Peer::getPort() const
  {
    return port_;
  }

  int Peer::getFd() const
  {
    return fd_;
  }

  void Peer::setTorrent(Core::Torrent* torrent)
  {
    torrent_ = torrent;
  }

} // namespace Network
