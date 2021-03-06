#include "torrent.hh"
#include "core/color.hh"
#include "core/url-utils.hh"

namespace Core
{
  Torrent::Torrent()
  { }

  Torrent::Torrent(const BDico& metaInfo, const std::string& fileName)
    : fileName_(fileName)
    , peerId_("-LHT01-1234568765011")
    , metaInfo_(metaInfo)
    , port_(80)
    , uploaded_(0)
    , downloaded_(0)
    , left_(0)
  {
    init();
  }

  void Torrent::init()
  {
    BDico infoDico = getType<BType_ptr, BDico>(metaInfo_.get("info"));
    //std::cout << infoDico << std::endl;
    auto files = infoDico.get("files");

    if (!files)
      left_ = getDecode<BType_ptr, BInteger,
			long long int>(infoDico.get("length"));
    else
    {
      BList filesList = getType<BType_ptr, BList>(files);
      for (auto i : filesList.getDecodedValue())
      {
	BDico file = getType<BType_ptr, BDico>(i);
        left_ += getDecode<BType_ptr, BInteger,
			   long long int>(file.get("length"));

        // cyp
        std::cout << i << std::endl;
        auto filePathList = getType<BType_ptr, BList>(file.get("path"));
        auto filePath = filePathList.getDecodedValue()[0];
        auto realPath = getType<BType_ptr, BString>(filePath);
        auto realLength = getType<BType_ptr, BInteger>(file.get("length"));
        files_.push_back(std::make_pair(realPath.getDecodedValue(),
                                        realLength.getDecodedValue()));
      }
    }

    pieces_ = getType<BType_ptr, BString>(infoDico.get("pieces")).getDecodedValue();

    pieces_length_ = getDecode<BType_ptr, BInteger,
			       long long int>(infoDico.get("piece length"));

    computeInfoHash(infoDico);
    computeEncodedInfoHash();
    computeAnnounce();
    Core::URLUtils urlUtils;
    urlGenerated_ = urlUtils.generateURL(*this);
  }

  void Torrent::computeInfoHash(BDico infoDico)
  {
    std::string encodedDico = infoDico.bEncode();
    std::string hash;
    unsigned char outBuffer[20];
    const unsigned char *inBuffer = (const unsigned char *)encodedDico.c_str();

    SHA1(inBuffer, encodedDico.size(), outBuffer);

    for (int i = 0; i < 20; ++i) {
      hash += outBuffer[i];
    }

    infoHash_ = hash;
  }

  void Torrent::computeEncodedInfoHash()
  {
    Core::URLUtils ur;
    encodedInfoHash_ = ur.percentEncode(infoHash_);
  }

  void Torrent::computeAnnounce()
  {
    auto announceListType = metaInfo_.get("announce-list");

    if (announceListType)
    {
      auto announceList = getType<BType_ptr, BList>(announceListType);
      for (auto i : announceList.getDecodedValue())
      {
	auto list = getType<BType_ptr, BList>(i);
	auto announce = getType<BType_ptr, BString>(list.getDecodedValue()[0]);
        announceList_.push_back(announce.getDecodedValue());
      }
    }
    else
      announceList_.push_back(getFromDico<BString,
			      std::string>(metaInfo_, "announce"));
  }

  void Torrent::dump()
  {
    Core::ColorModifier clgreen(Core::ColorCode::FG_LIGHT_BLUE);
    Core::ColorModifier cyan(Core::ColorCode::FG_CYAN);
    Core::ColorModifier cdef(Core::ColorCode::FG_DEFAULT);
    std::cout << clgreen << "Torrent Description\n" << cdef << std::endl;
    std::cout << cyan << "PeerId :\n\t" << cdef << peerId_ << std::endl;
    std::cout << cyan << "Info hash :\n\t " << cdef << infoHash_ << std::endl;
    std::cout << cyan << " Encoded Info hash :\n\t " <<
      cdef << encodedInfoHash_ << std::endl;
    std::cout << cyan << "uploaded :\n\t" << cdef << uploaded_ << std::endl;
    std::cout << cyan << "downloaded :\n\t" << cdef << downloaded_ << std::endl;
    std::cout << cyan << "left :\n\t" << cdef << left_ << std::endl;

    std::cout << cyan << "Announce List :\n\t" << cdef << std::endl;
    for (auto announce : announceList_)
      std::cout << "\t" << announce << std::endl;
    std::cout << std::endl;
  }

  // Getter
  std::vector<std::string> Torrent::getAnnounceList() const
  {
    return announceList_;
  }

  std::string Torrent::getFileName() const
  {
    return fileName_;
  }

  std::string Torrent::getEncodedInfoHash() const
  {
    return encodedInfoHash_;
  }

  std::string Torrent::getInfoHash() const
  {
    return infoHash_;
  }

  std::string Torrent::getPeerId() const
  {
    return peerId_;
  }

  std::string Torrent::getUrlGenerated() const
  {
    return urlGenerated_;
  }

  BDico Torrent::getMetaInfo() const
  {
    return metaInfo_;
  }

  long long int Torrent::getPiecesLength() const
  {
    return pieces_length_;
  }

  const std::string& Torrent::getPiecesHash() const
  {
    return pieces_;
  }

  size_t Torrent::getNbPieces() const
  {
    return (left_ / pieces_length_) + ((left_ % pieces_length_) ? 1 : 0);
  }

  long int Torrent::getPort() const
  {
    return port_;
  }

  long long int Torrent::getUploaded() const
  {
    return uploaded_;
  }

  long long int Torrent::getDownloaded() const
  {
    return downloaded_;
  }

  long long int Torrent::getLeft() const
  {
    return left_;
  }

  std::vector<std::pair<std::string, long long int>> Torrent::getFiles() const
  {
    return files_;
  }

} // namespace Network
