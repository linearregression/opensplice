
#include "d_nameSpaces.h"
#include "d__nameSpaces.h"
#include "d_nameSpace.h"
#include "d_networkAddress.h"
#include "d__nameSpace.h"
#include "d_admin.h"
#include "d_message.h"
#include "d_table.h"
#include "os.h"

d_nameSpaces
d_nameSpacesNew(
    d_admin admin,
    d_nameSpace nameSpace,
    d_quality initialQuality,
    c_ulong total)
{
    d_nameSpaces ns = NULL;
    d_durability durability;
    d_networkAddress master;
    
    if(nameSpace){
        ns = d_nameSpaces(os_malloc(C_SIZEOF(d_nameSpaces)));
        
        if(ns){
            durability = d_adminGetDurability(admin);
            master     = d_networkAddressUnaddressed();
            d_messageInit(d_message(ns), admin);
            
            ns->durabilityKind             = d_nameSpaceGetDurabilityKind(nameSpace);
            ns->alignmentKind              = d_nameSpaceGetAlignmentKind(nameSpace);
            ns->partitions                 = d_nameSpaceGetPartitions(nameSpace);
            ns->total                      = total;
            ns->initialQuality.seconds     = initialQuality.seconds;
            ns->initialQuality.nanoseconds = initialQuality.nanoseconds;
            ns->master.systemId            = master->systemId;
            ns->master.localId             = master->localId;
            ns->master.lifecycleId         = master->lifecycleId;
            
            d_networkAddressFree(master);
        }
    }
    return ns;
}

d_networkAddress
d_nameSpacesGetMaster(
        d_nameSpaces nameSpaces)
{
    d_networkAddress addr = NULL;
    
    if(nameSpaces){
        addr = d_networkAddressNew( nameSpaces->master.systemId,
                                    nameSpaces->master.localId,
                                    nameSpaces->master.lifecycleId);
    }
    return addr;
}

void
d_nameSpacesSetMaster(
    d_nameSpaces nameSpaces,
    d_networkAddress master)
{
    if(nameSpaces && master){
        nameSpaces->master.systemId    = master->systemId;
        nameSpaces->master.localId     = master->localId;
        nameSpaces->master.lifecycleId = master->lifecycleId;
    }
    return;
}


d_alignmentKind
d_nameSpacesGetAlignmentKind(
    d_nameSpaces nameSpaces)
{
    d_alignmentKind kind = D_ALIGNEE_INITIAL_AND_ALIGNER;
    
    if(nameSpaces){
        kind = nameSpaces->alignmentKind;
    }
    return kind;
}

d_durabilityKind
d_nameSpacesGetDurabilityKind(
    d_nameSpaces nameSpaces)
{
    d_durabilityKind kind = D_DURABILITY_ALL;
    
    if(nameSpaces){
        kind = nameSpaces->durabilityKind;
    }
    return kind;
}

c_bool
d_nameSpacesIsAligner(
    d_nameSpaces nameSpaces)
{
    c_bool aligner = FALSE;
    
    if(nameSpaces){
        if(nameSpaces->alignmentKind == D_ALIGNEE_INITIAL_AND_ALIGNER){
            aligner = TRUE;
        }
    }
    return aligner;
}

c_ulong
d_nameSpacesGetTotal(
    d_nameSpaces nameSpaces)
{
    c_ulong total = 0;
    
    if(nameSpaces){
        total = nameSpaces->total;
    }
    return total;
}

c_char*
d_nameSpacesGetPartitions(
    d_nameSpaces nameSpaces)
{
    c_char* partitions = NULL;
    
    if(nameSpaces){
        if(nameSpaces->partitions){
            partitions = os_strdup(nameSpaces->partitions);
        }
    }
    return partitions;
}

void
d_nameSpacesFree(
    d_nameSpaces nameSpaces)
{
    
    if(nameSpaces){
        if(nameSpaces->partitions){
            os_free(nameSpaces->partitions);
            nameSpaces->partitions = NULL;
        }
        d_messageDeinit(d_message(nameSpaces));
        os_free(nameSpaces);
    }
}

d_quality
d_nameSpacesGetInitialQuality(
    d_nameSpaces nameSpaces)
{
    d_quality quality;
    
    if(nameSpaces){
        quality.seconds     = nameSpaces->initialQuality.seconds;
        quality.nanoseconds = nameSpaces->initialQuality.nanoseconds;
    }
    return quality;
}

int
d_nameSpacesCompare(
    d_nameSpaces ns1,
    d_nameSpaces ns2)
{
    int r;
    
    if((!ns1) && (!ns2)){
        r = 0;
    } else if(!ns1){
        r = 1;
    } else if(!ns2){
        r = -1;
    } else if(ns1->alignmentKind != ns2->alignmentKind){
        if(((c_ulong)ns1->alignmentKind) > ((c_ulong)ns2->alignmentKind)){
            r = 1;
        } else {
            r = -1;
        }
    } else if(ns1->durabilityKind != ns2->durabilityKind) {
        if(((c_ulong)ns1->durabilityKind) > ((c_ulong)ns2->durabilityKind)){
            r = 1;
        } else {
            r = -1;
        }
    } else if((!ns1->partitions) && (!ns2->partitions)){
        r = 0;
    } else if(!ns1->partitions){
        r = 1;
    } else if(!ns2->partitions){
        r = -1;
    } else {
        r = strcmp(ns1->partitions, ns2->partitions);
    }
    return r;
}