//Class header
#include "CMPEG2PSDemuxer.h"
//Local
#include "../../../Codecs/MPEG/MPEG1.h"
#include "../../../Codecs/MPEG/CMPEGAudioParser.h"
#include "../../../Codecs/MPEG/CMPEGVideoParser.h"
//Definitions
#define MPEG_SCR 90000 //90kHz
#define MPEG1_STARTCODE_SYSTEMHEADER 0xBB
#define MPEG1_STARTCODE_PRIVATESTREAM1 0xBD
#define MPEG1_PADDINGSTREAM_STREAMID 0xBE
#define MPEG1_STARTCODE_PRIVATESTREAM2 0xBF

//Private methods
AStream *CMPEG2PSDemuxer::MapDVDPrivateStream1(byte subStreamNumber)
{
	AStream *pStream;

	//greatfully dvd stream ids do not overlap with MPEG ones

	if(this->streamMap.Contains(subStreamNumber))
		return this->GetStream(this->streamMap[subStreamNumber]);

	pStream = nullptr;

	if(subStreamNumber >= 0x20 && subStreamNumber <= 0x3F)
	{
		pStream = new CSubtitleStream;

		pStream->SetCodec(ICodec::GetCodec(ECodecId::DVD_Subtitle));

		this->streamMap[subStreamNumber] = ADemuxer::AddStream(pStream);
	}
	else if(subStreamNumber >= 0x80 && subStreamNumber <= 0x87)
	{
		pStream = new CAudioStream;

		pStream->SetCodec(ICodec::GetCodec(ECodecId::AC3));

		this->streamMap[subStreamNumber] = ADemuxer::AddStream(pStream);
	}

	return pStream;
}

AStream *CMPEG2PSDemuxer::MapStream(uint8 streamId)
{
	AStream *pStream;

	if(this->streamMap.Contains(streamId))
		return this->GetStream(this->streamMap[streamId]);

	pStream = nullptr;

	if(IN_RANGE(streamId, 0xC0, 0xDF))
	{
		pStream = new CAudioStream;

		pStream->SetParser(new CMPEGAudioParser);

		pStream->timeScale = CFraction(1, MPEG_SCR);

		this->streamMap[streamId] = ADemuxer::AddStream(pStream);
	}
	else if(IN_RANGE(streamId, 0xE0, 0xEF))
	{
		pStream = new CVideoStream;

		pStream->SetParser(new CMPEGVideoParser);

		pStream->timeScale = CFraction(1, MPEG_SCR);

		this->streamMap[streamId] = ADemuxer::AddStream(pStream);
	}

	return pStream;
}

/*
sources:
	for MPEG-1: 2.4.3.2
	for MPEG-2: Table 2-33 – Program Stream pack header
*/
void CMPEG2PSDemuxer::ReadPackHeader()
{
	byte b;
	uint32 muxRate;
	uint64 scr;

	b = this->refInput.ReadByte();
	if((b & 0xC0) == 0x40)
	{
		uint8 pack_stuffing_length;
		uint16 scrExt;

		//MPEG-2
		scr = uint64(b & 0x38) << 27;
		scr |= uint64(b & 3) << 28;

		scr |= uint64(this->refInput.ReadByte()) << 20;

		b = this->refInput.ReadByte();
		scr |= uint64(b & 0xF8) << 12;
		scr |= uint64(b & 3) << 13;

		scr |= uint64(this->refInput.ReadByte()) << 5;

		b = this->refInput.ReadByte();
		scr |= uint64(b & 0xF8) >> 3;

		scrExt = uint16(b & 3) << 7;
		scrExt |= this->refInput.ReadByte() >> 1;

		muxRate = uint32(this->refInput.ReadByte()) << 14;
		muxRate |= uint32(this->refInput.ReadByte()) << 6;
		muxRate |= this->refInput.ReadByte() >> 2;

		pack_stuffing_length = this->refInput.ReadByte() & 7;
		this->refInput.Skip(pack_stuffing_length);
	}
	else
	{
		//MPEG-1
		scr = uint64(b & 0xE) << 29;
		scr |= uint64(this->refInput.ReadByte()) << 22;
		scr |= uint64(this->refInput.ReadByte() & 0xFE) << 14;
		scr |= uint64(this->refInput.ReadByte()) << 7;
		scr |= this->refInput.ReadByte() >> 1;

		muxRate = uint64(this->refInput.ReadByte() & 0x7F) << 15;
		muxRate |= uint64(this->refInput.ReadByte()) << 7;
		muxRate |= this->refInput.ReadByte() >> 1;
	}
}

/*
MPEG-1: 2.4.3.3
MPEG-2: 2.4.3.6
*/
uint8 CMPEG2PSDemuxer::ReadPESExtendedHeader(uint64 &refPts)
{
	uint8 length;
	byte b;

	b = this->refInput.ReadByte();
	length = 1;

	if((b & 0xC0) == 0x80)
	{
		bool ESCR_flag, ES_rate_flag, DSM_trick_mode_flag, additional_copy_info_flag, PES_CRC_flag, PES_extension_flag;
		uint8 PTS_DTS_flags, PES_header_data_length, nStuffingBytes;

		//MPEG-2

		//first byte (b)
		//PES_scrambling_control 2bits
		//PES_priority 1bit
		//data_alignment_indicator 1bit
		//copyright 1bit
		//original_or_copy 1bit

		//second byte
		b = this->refInput.ReadByte();
		length++;

		PTS_DTS_flags = (b & 0xC0) >> 6;
		ESCR_flag = (b & 0x20) != 0;
		ES_rate_flag = (b & 0x10) != 0;
		DSM_trick_mode_flag = (b & 8) != 0;
		additional_copy_info_flag = (b & 4) != 0;
		PES_CRC_flag = (b & 2) != 0;
		PES_extension_flag = (b & 1) != 0;

		PES_header_data_length = this->refInput.ReadByte();
		length++;

		if(PTS_DTS_flags & 2)
		{
			b = this->refInput.ReadByte();
			length++;

			refPts = ((b & 0xE) << 29);
			refPts |= ((uint64)refInput.ReadByte() << 22);
			refPts |= ((uint64)(refInput.ReadByte() >> 1) << 15);
			refPts |= ((uint64)refInput.ReadByte() << 7);
			refPts |= (refInput.ReadByte() >> 1);
			length += 4;

			if(PTS_DTS_flags == 3)
			{
				refInput.Skip(5); //dts
				length += 5;
			}
		}


		if(ESCR_flag)
		{
			this->refInput.Skip(6);
			length += 6;
		}

		if(ES_rate_flag)
		{
			this->refInput.Skip(3);
			length += 3;
		}

		if(DSM_trick_mode_flag)
		{
			this->refInput.Skip(1);
			length++;
		}

		if(additional_copy_info_flag)
		{
			this->refInput.Skip(1);
			length++;
		}

		if(PES_CRC_flag)
		{
			this->refInput.Skip(2);
			length += 2;
		}

		if(PES_extension_flag)
		{
			bool PES_private_data_flag, pack_header_field_flag, program_packet_sequence_counter_flag, P_STD_buffer_flag, PES_extension_flag_2;

			b = this->refInput.ReadByte();
			length++;

			PES_private_data_flag = (b & 0x80) != 0;
			pack_header_field_flag = (b & 0x40) != 0;
			program_packet_sequence_counter_flag = (b & 0x20) != 0;
			P_STD_buffer_flag = (b & 0x10) != 0;
			PES_extension_flag_2 = (b & 1) != 0;

			if(PES_private_data_flag)
			{
				this->refInput.Skip(16);
				length += 16;
			}

			if(pack_header_field_flag)
			{
				length += 1 + this->refInput.ReadByte();
				this->ReadPackHeader();
			}

			if(program_packet_sequence_counter_flag)
			{
				this->refInput.Skip(2);
				length += 2;
			}

			if(P_STD_buffer_flag)
			{
				this->refInput.Skip(2);
				length += 2;
			}

			if(PES_extension_flag_2)
			{
				uint8 PES_extension_field_length;

				PES_extension_field_length = this->refInput.ReadByte() & 0x7F;
				length++;
				this->refInput.Skip(PES_extension_field_length);
				length += PES_extension_field_length;
			}
		}

		//stuffing bytes
		nStuffingBytes = length - PES_header_data_length - 3;
		this->refInput.Skip(nStuffingBytes);

		length += nStuffingBytes;
	}
	else
	{
		//skip stuffing bytes
		while(b & 0x80)
		{
			b = this->refInput.ReadByte();
			length++;
		}

		//bitstring '01' implies a STD-Buffer size field
		if((b & 0xC0) == 0x40)
		{
			refInput.Skip(1);
			length++;

			//check for next field
			b = refInput.ReadByte();
			length++;
		}

		/*
		bitstring '0010' implies a PTS field
		bitstring '0011' implies a PTS followed by a DTS field
		*/
		if((b & 0xE0) == 0x20) //'001'
		{
			refPts = ((b & 0xE) << 29);
			refPts |= ((uint64)refInput.ReadByte() << 22);
			refPts |= ((uint64)(refInput.ReadByte() >> 1) << 15);
			refPts |= ((uint64)refInput.ReadByte() << 7);
			refPts |= (refInput.ReadByte() >> 1);
			length += 4;

			if(b & 0x10) //'0011'
			{
				refInput.Skip(5); //dts
				length += 5;
			}
		}
	}

	return length;
}

/*
source: Table 2-34 – Program Stream system header
*/
void CMPEG2PSDemuxer::ReadSystemHeader()
{
	uint8 nAudioStreams, nVideoStreams, streamId;
	uint16 headerLength;
	uint32 rate_bound;

	headerLength = this->refInput.ReadUInt16BE();

	CBitInputStream bitInput(refInput);

	ASSERT(bitInput.Read(1) == 1); //marker
	rate_bound = (uint32)bitInput.Read(22);
	ASSERT(bitInput.Read(1) == 1); //marker

	nAudioStreams = (uint8)bitInput.Read(6);

	bitInput.Read(1); //fixed flag
	bitInput.Read(1); //CSPS flag
	bitInput.Read(1); //system_audio_lock_flag
	bitInput.Read(1); //system_video_lock_flag
	ASSERT(bitInput.Read(1) == 1); //marker

	nVideoStreams = (uint8)bitInput.Read(5);

	bitInput.Read(1); //packet_rate_restriction_flag
	ASSERT(bitInput.Read(7) == 0x7F); //reserved

	headerLength -= 6; //so far we read 6 bytes
	while(headerLength)
	{
		streamId = (uint8)bitInput.Read(8); //streamId

		ASSERT(bitInput.Read(2) == 3); //fixed '11'
		bitInput.Read(1); //P-STD_buffer_bound_scale
		bitInput.Read(13); //P-STD_buffer_size_bound

		if(streamId != 0xB8 && streamId != 0xB9) //P-STD stuff is for all video/audio streams
		{
			this->MapStream(streamId);
		}

		headerLength -= 3;
	}
}

void CMPEG2PSDemuxer::Resync(byte &refStartCode)
{
	uint32 startCode;
	
	//check until we find a PES start code
	while(true)
	{
		startCode = UINT32_MAX;
		FindNextStartCode(startCode, this->refInput);
		if(this->refInput.HitEnd())
			break;
		refStartCode = startCode & 0xFF;
		
		if(IN_RANGE(refStartCode, 0xBD, 0xEF)) //PES packet
			break;
	}
}

//Public methods
void CMPEG2PSDemuxer::ReadHeader()
{
	uint64 currentOffset;
	SPacket packet;

	//mpeg-1 systems/mpeg2 ps has no header but we can read some packets to get streams

	currentOffset = this->refInput.GetCurrentOffset();

	for(uint32 i = 0; i < 1000; i++)
	{
		this->ReadPacket(packet);
		FreePacket(packet);
	}

	this->refInput.SetCurrentOffset(currentOffset);
}

/*
source: Table 2-17 – PES packet
*/
bool CMPEG2PSDemuxer::ReadPacket(SPacket &refPacket)
{
	byte startCode;

	InitPacket(refPacket);

	//read packets until we have found a PES header
	while(true)
	{
		this->Resync(startCode);

		//check if we are at the end of stream
		if(this->refInput.HitEnd())
			return false;

		if(IN_RANGE(startCode, 0xBD, 0xEF))
		{
			uint16 packetLength;

			//found a PES packet

			packetLength = this->refInput.ReadUInt16BE();

			if(startCode == MPEG1_STARTCODE_PRIVATESTREAM1)
			{
				byte subStreamNumber;

				packetLength -= this->ReadPESExtendedHeader(refPacket.pts);

				subStreamNumber = this->refInput.ReadByte();
				packetLength--;
				if(this->MapDVDPrivateStream1(subStreamNumber))
				{
					//fill out packet info that we have from PES header
					refPacket.size = packetLength;
					refPacket.streamIndex = this->streamMap[startCode];

					break; //we have found a data packet
				}
			}
			else if(startCode == MPEG1_PADDINGSTREAM_STREAMID)
			{
				//contains no info... only padding
				this->refInput.Skip(packetLength);
			}
			else if(startCode == MPEG1_STARTCODE_PRIVATESTREAM2)
			{
				//might contain interesting data but for the moment skip
				this->refInput.Skip(packetLength);
			}
			else if(IN_RANGE(startCode, 0xC0, 0xEF))
			{
				//data!

				this->MapStream(startCode);

				packetLength -= this->ReadPESExtendedHeader(refPacket.pts);

				//fill out packet info that we have from PES header
				refPacket.size = packetLength;
				refPacket.streamIndex = this->streamMap[startCode];

				break; //we have found the desired packet
			}
		}
		else
		{
			//another packet
			switch(startCode)
			{
			case MPEG1_STARTCODE_PACKHEADER:
				this->ReadPackHeader();
				break;
			case MPEG1_STARTCODE_SYSTEMHEADER:
				this->ReadSystemHeader();
				break;
			default:
				ASSERT(0);
			}
		}
	}

	//we have found a packet, now read
	refPacket.pData = (byte *)MemAlloc(refPacket.size);
	this->refInput.ReadBytes(refPacket.pData, refPacket.size);

	return true;
}