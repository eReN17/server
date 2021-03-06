#include "botpch.h"
#include "../../playerbot.h"
#include "BuyAction.h"
#include "../ItemVisitors.h"
#include "../values/ItemCountValue.h"

using namespace ai;

bool BuyAction::Execute(Event event)
{
    string link = event.getParam();

    ItemIds itemIds = chat->parseItems(link);
    if (itemIds.empty())
        return false;

    Player* master = GetMaster();

    if (!master)
        return false;

    ObjectGuid vendorguid = master->GetSelectionGuid();
    if (!vendorguid)
        return false;

    Creature *pCreature = bot->GetNPCIfCanInteractWith(vendorguid,UNIT_NPC_FLAG_VENDOR);
    if (!pCreature)
    {
        ai->TellMaster("Cannot talk to vendor");
        return false;
    }

    VendorItemData const* tItems = pCreature->GetVendorItems();
    if (!tItems)
    {
        ai->TellMaster("This vendor has no items");
        return false;
    }

    for (ItemIds::iterator i = itemIds.begin(); i != itemIds.end(); i++)
    {
        for (uint32 slot = 0; slot < tItems->GetItemCount(); slot++)
        {
            if (tItems->GetItem(slot)->item == *i)
            {
                bot->BuyItemFromVendor(vendorguid, *i, 1, NULL_BAG, NULL_SLOT);
                ai->TellMaster("Bought item");
            }
        }
    }

    return true;
}
