pushd %~dp0

protoc.exe -I=./ --cpp_out=./ ./Enum.proto
protoc.exe -I=./ --cpp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ ./Protocol.proto

GenPackets.exe --path=./Protocol.proto --output=ClientPacketHandler --recv=S_ --send=C_
GenPackets.exe --path=./Protocol.proto --output=ServerPacketHandler --recv=C_ --send=S_

IF ERRORLEVEL 1 PAUSE

XCOPY /Y Enum.pb.h "D:/Unreal/UE5_Spectrum/Server/GameServer"
XCOPY /Y Enum.pb.cc "D:/Unreal/UE5_Spectrum/Server/GameServer"
XCOPY /Y Struct.pb.h "D:/Unreal/UE5_Spectrum/Server/GameServer"
XCOPY /Y Struct.pb.cc "D:/Unreal/UE5_Spectrum/Server/GameServer"
XCOPY /Y Protocol.pb.h "D:/Unreal/UE5_Spectrum/Server/GameServer"
XCOPY /Y Protocol.pb.cc "D:/Unreal/UE5_Spectrum/Server/GameServer"
XCOPY /Y ServerPacketHandler.h "D:/Unreal/UE5_Spectrum/Server/GameServer"

XCOPY /Y Enum.pb.h "D:/Unreal/UE5_Spectrum/Spectrum/Source/Spectrum/Network"
XCOPY /Y Enum.pb.cc "D:/Unreal/UE5_Spectrum/Spectrum/Source/Spectrum/Network"
XCOPY /Y Struct.pb.h "D:/Unreal/UE5_Spectrum/Spectrum/Source/Spectrum/Network"
XCOPY /Y Struct.pb.cc "D:/Unreal/UE5_Spectrum/Spectrum/Source/Spectrum/Network"
XCOPY /Y Protocol.pb.h "D:/Unreal/UE5_Spectrum/Spectrum/Source/Spectrum/Network"
XCOPY /Y Protocol.pb.cc "D:/Unreal/UE5_Spectrum/Spectrum/Source/Spectrum/Network"
XCOPY /Y ClientPacketHandler.h "D:/Unreal/UE5_Spectrum/Spectrum/Source/Spectrum"

DEL /Q /F *.pb.h
DEL /Q /F *.pb.cc
DEL /Q /F *.h

PAUSE