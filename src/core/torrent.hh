#pragma once

#include <openssl/sha.h>
#include <stddef.h>

#include "../bencode/bencode-dico.hh"
#include "../bencode/bencode-utils.hh"

namespace Core
{
  using namespace BEncode;

  class Torrent
  {
  public:
    // Constructor & Destructor
    Torrent();
    Torrent(const BDico& metaInfo);
    ~Torrent() = default;

    // Action
    void init();
    void computeInfoHash(BDico infoDico);
    void computeEncodedInfoHash();
    void computeAnnounce();
    void dump();

    // Getter
    std::vector<std::string> getAnnounceList() const;
    std::string getEncodedInfoHash() const;
    std::string getInfoHash() const;
    std::string getPeerId() const;
    BDico getMetaInfo() const;
    size_t getNbPieces() const;
    long int getPort() const;
    long long int getPiecesLength() const;
    long long int getUploaded() const;
    long long int getDownloaded() const;
    long long int getLeft() const;

  private:
    std::vector<std::string> announceList_;

    std::string infoHash_;
    std::string encodedInfoHash_;
    std::string peerId_;
    BDico metaInfo_;
    size_t pieces_length_;
    long int port_;
    long long int uploaded_;
    long long int downloaded_;
    long long int left_; // Total size
  };

} // namespace Network
