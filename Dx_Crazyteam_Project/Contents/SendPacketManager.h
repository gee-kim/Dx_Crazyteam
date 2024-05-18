#pragma once

// ���� :
class AMapObject;
class USendPacketManager
{
public:
	// constrcuter destructer
	USendPacketManager();
	~USendPacketManager();

	// delete Function
	USendPacketManager(const USendPacketManager& _Other) = delete;
	USendPacketManager(USendPacketManager&& _Other) noexcept = delete;
	USendPacketManager& operator=(const USendPacketManager& _Other) = delete;
	USendPacketManager& operator=(USendPacketManager&& _Other) noexcept = delete;

	static void SendItemSpawnPacket(std::shared_ptr<AMapObject> _NetObject, EItemType _ItemType, POINT _CurPos);
	static void SendItemReleasePacket(AMapObject* _NetObject, POINT _CurPos);

protected:

private:

};

