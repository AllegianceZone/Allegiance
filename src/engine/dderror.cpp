//////////////////////////////////////////////////////////////////////////////
//
// HRESULT converter
//
//////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "ddraw.h"
//#include "d3d.h"
//#include "d3drm.h"
#include "dinput.h"

#ifdef _DEBUG

class HRESULTInfo {
public:
    HRESULT hresult;
	wchar_t* pszError;
	wchar_t* pszText;
};

HRESULTInfo gpinfo[] =
{
    /*************************/
    /*** DirectDraw Errors ***/
    /*************************/

    {DDERR_ALREADYINITIALIZED, L"DDERR_ALREADYINITIALIZED",
        L"This object is already initialized"},

    {DDERR_CANNOTATTACHSURFACE, L"DDERR_CANNOTATTACHSURFACE",
        L"This surface can not be attached to the requested surface"},

    {DDERR_CANNOTDETACHSURFACE, L"DDERR_CANNOTDETACHSURFACE",
        L"This surface can not be detached from the requested surface"},

    {DDERR_CURRENTLYNOTAVAIL, L"DDERR_CURRENTLYNOTAVAIL",
        L"Support is currently not available"},

    {DDERR_EXCEPTION, L"DDERR_EXCEPTION",
        L"An exception was encountered while performing the requested "
        L"operation"},

    {DDERR_GENERIC, L"DDERR_GENERIC",
        L"Generic failure"},

    {DDERR_HEIGHTALIGN, L"DDERR_HEIGHTALIGN",
        L"Height of rectangle provided is not a multiple of required "
        L"alignment"},

    {DDERR_INCOMPATIBLEPRIMARY, L"DDERR_INCOMPATIBLEPRIMARY",
        L"Unable to match primary surface creation request with existing "
        L"primary surface"},

    {DDERR_INVALIDCAPS, L"DDERR_INVALIDCAPS",
        L"One or more of the caps bits passed to the callback are incorrect"},

    {DDERR_INVALIDCLIPLIST, L"DDERR_INVALIDCLIPLIST",
        L"DirectDraw does not support provided Cliplist"},

    {DDERR_INVALIDMODE, L"DDERR_INVALIDMODE",
        L"DirectDraw does not support the requested mode"},

    {DDERR_INVALIDOBJECT, L"DDERR_INVALIDOBJECT",
        L"DirectDraw received a pointer that was an invalid DIRECTDRAW object"},

    {DDERR_INVALIDPARAMS, L"DDERR_INVALIDPARAMS",
        L"One or more of the parameters passed to the method are incorrect."},

    {DDERR_INVALIDPIXELFORMAT, L"DDERR_INVALIDPIXELFORMAT",
        L"Pixel format was invalid as specified"},

    {DDERR_INVALIDRECT, L"DDERR_INVALIDRECT",
        L"Rectangle provided was invalid"},

    {DDERR_LOCKEDSURFACES, L"DDERR_LOCKEDSURFACES",
        L"Operation could not be carried out because one or more surfaces "
        L"are locked"},

    {DDERR_NO3D, L"DDERR_NO3D",
        L"There is no 3D present"},

    {DDERR_NOALPHAHW, L"DDERR_NOALPHAHW",
        L"Operation could not be carried out because there is no alpha "
        L"accleration hardware present or available"},

    {DDERR_NOCLIPLIST, L"DDERR_NOCLIPLIST",
        L"No clip list available"},

    {DDERR_NOCOLORCONVHW, L"DDERR_NOCOLORCONVHW",
        L"Operation could not be carried out because there is no color "
        L"conversion hardware present or available"},

    {DDERR_NOCOOPERATIVELEVELSET, L"DDERR_NOCOOPERATIVELEVELSET",
        L"Create function called without DirectDraw object method "
        L"SetCooperativeLevel being called"},

    {DDERR_NOCOLORKEY, L"DDERR_NOCOLORKEY",
        L"Surface doesn't currently have a color key"},

    {DDERR_NOCOLORKEYHW, L"DDERR_NOCOLORKEYHW",
        L"Operation could not be carried out because there is no "
        L"hardware support of the destination color key"},

    {DDERR_NODIRECTDRAWSUPPORT, L"DDERR_NODIRECTDRAWSUPPORT",
        L"No DirectDraw support possible with current display driver"},

    {DDERR_NOEXCLUSIVEMODE, L"DDERR_NOEXCLUSIVEMODE",
        L"Operation requires the application to have exclusive mode "
        L"but the application does not have exclusive mode"},

    {DDERR_NOFLIPHW, L"DDERR_NOFLIPHW",
        L"Flipping visible surfaces is not supported"},

    {DDERR_NOGDI, L"DDERR_NOGDI",
        L"There is no GDI present"},

    {DDERR_NOMIRRORHW, L"DDERR_NOMIRRORHW",
        L"Operation could not be carried out because there is "
        L"no hardware present or available"},

    {DDERR_NOTFOUND, L"DDERR_NOTFOUND",
        L"Requested item was not found"},

    {DDERR_NOOVERLAYHW, L"DDERR_NOOVERLAYHW",
        L"Operation could not be carried out because there is "
        L"no overlay hardware present or available"},

    {DDERR_NORASTEROPHW, L"DDERR_NORASTEROPHW",
        L"Operation could not be carried out because there is "
        L"no appropriate raster op hardware present or available"},

    {DDERR_NOROTATIONHW, L"DDERR_NOROTATIONHW",
        L"Operation could not be carried out because there is "
        L"no rotation hardware present or available"},

    {DDERR_NOSTRETCHHW, L"DDERR_NOSTRETCHHW",
        L"Operation could not be carried out because there is "
        L"no hardware support for stretching"},

    {DDERR_NOT4BITCOLOR, L"DDERR_NOT4BITCOLOR",
        L"DirectDrawSurface is not in 4 bit color palette and "
        L"the requested operation requires 4 bit color palette"},

    {DDERR_NOT4BITCOLORINDEX, L"DDERR_NOT4BITCOLORINDEX",
        L"DirectDrawSurface is not in 4 bit color index palette "
        L"and the requested operation requires 4 bit color index palette"},

    {DDERR_NOT8BITCOLOR, L"DDERR_NOT8BITCOLOR",
        L"DirectDraw Surface is not in 8 bit color mode "
        L"and the requested operation requires 8 bit color"},

    {DDERR_NOTEXTUREHW, L"DDERR_NOTEXTUREHW",
        L"Operation could not be carried out because there is "
        L"no texture mapping hardware present or available"},

    {DDERR_NOVSYNCHW, L"DDERR_NOVSYNCHW",
        L"Operation could not be carried out because there is "
        L"no hardware support for vertical blank synchronized operations"},

    {DDERR_NOZBUFFERHW, L"DDERR_NOZBUFFERHW",
        L"Operation could not be carried out because there is "
        L"no hardware support for zbuffer blting"},

    {DDERR_NOZOVERLAYHW, L"DDERR_NOZOVERLAYHW",
        L"Overlay surfaces could not be z layered based on their "
        L"BltOrder because the hardware does not support z layering of overlays"},

    {DDERR_OUTOFCAPS, L"DDERR_OUTOFCAPS",
        L"The hardware needed for the requested operation has "
        L"already been allocated"},

    {DDERR_OUTOFMEMORY, L"DDERR_OUTOFMEMORY",
        L"DirectDraw does not have enough memory to perform the operation"},

    {DDERR_OUTOFVIDEOMEMORY, L"DDERR_OUTOFVIDEOMEMORY",
        L"DirectDraw does not have enough memory to perform the operation"},

    {DDERR_OVERLAYCANTCLIP, L"DDERR_OVERLAYCANTCLIP",
        L"hardware does not support clipped overlays"},

    {DDERR_OVERLAYCOLORKEYONLYONEACTIVE, L"DDERR_OVERLAYCOLORKEYONLYONEACTIVE",
        L"Can only have ony color key active at one time for overlays"},

    {DDERR_PALETTEBUSY, L"DDERR_PALETTEBUSY",
        L"Access to this palette is being refused because the palette "
        L"is already locked by another thread"},

    {DDERR_COLORKEYNOTSET, L"DDERR_COLORKEYNOTSET",
        L"No src color key specified for this operation"},

    {DDERR_SURFACEALREADYATTACHED, L"DDERR_SURFACEALREADYATTACHED",
        L"This surface is already attached to the surface it is "
        L"being attached to"},

    {DDERR_SURFACEALREADYDEPENDENT, L"DDERR_SURFACEALREADYDEPENDENT",
        L"This surface is already a dependency of the surface it "
        L"is being made a dependency of"},

    {DDERR_SURFACEBUSY, L"DDERR_SURFACEBUSY",
        L"Access to this surface is being refused because the surface "
        L"is already locked by another thread"},

    {DDERR_SURFACEISOBSCURED, L"DDERR_SURFACEISOBSCURED",
        L"Access to Surface refused because Surface is obscured"},

    {DDERR_SURFACELOST, L"DDERR_SURFACELOST",
        L"The DIRECTDRAWSURFACE object representing this surface "
        L"should have Restore called on it.  Access to this surface is "
        L"being refused because the surface is gone"},

    {DDERR_SURFACENOTATTACHED, L"DDERR_SURFACENOTATTACHED",
        L"The requested surface is not attached"},

    {DDERR_TOOBIGHEIGHT, L"DDERR_TOOBIGHEIGHT",
        L"Height requested by DirectDraw is too large"},

    {DDERR_TOOBIGSIZE, L"DDERR_TOOBIGSIZE",
        L"Size requested by DirectDraw is too large.  The individual height "
        L"and width are OK"},

    {DDERR_TOOBIGWIDTH, L"DDERR_TOOBIGWIDTH",
        L"Width requested by DirectDraw is too large"},

    {DDERR_UNSUPPORTED, L"DDERR_UNSUPPORTED",
        L"Action not supported"},

    {DDERR_UNSUPPORTEDFORMAT, L"DDERR_UNSUPPORTEDFORMAT",
        L"FOURCC format requested is unsupported by DirectDraw"},

    {DDERR_UNSUPPORTEDMASK, L"DDERR_UNSUPPORTEDMASK",
        L"Bitmask in the pixel format requested is unsupported by DirectDraw"},

    {DDERR_VERTICALBLANKINPROGRESS, L"DDERR_VERTICALBLANKINPROGRESS",
        L"vertical blank is in progress"},

    {DDERR_WASSTILLDRAWING, L"DDERR_WASSTILLDRAWING",
        L"Informs DirectDraw that the previous Blt which is "
        L"transfering information to or from this Surface is incomplete"},

    {DDERR_XALIGN, L"DDERR_XALIGN",
        L"Rectangle provided was not horizontally aligned on required boundary"},

    {DDERR_INVALIDDIRECTDRAWGUID, L"DDERR_INVALIDDIRECTDRAWGUID",
        L"The GUID passed to DirectDrawCreate is not a valid DirectDraw driver "
        L"identifier"},

    {DDERR_DIRECTDRAWALREADYCREATED, L"DDERR_DIRECTDRAWALREADYCREATED",
        L"A DirectDraw object representing this driver has already been "
        L"created for this process"},

    {DDERR_NODIRECTDRAWHW, L"DDERR_NODIRECTDRAWHW",
        L"A hardware only DirectDraw object creation was attempted "
        L"but the driver did not support any hardware"},

    {DDERR_PRIMARYSURFACEALREADYEXISTS, L"DDERR_PRIMARYSURFACEALREADYEXISTS",
        L"This process already has created a primary surface"},

    {DDERR_NOEMULATION, L"DDERR_NOEMULATION",
        L"Software emulation not available"},

    {DDERR_REGIONTOOSMALL, L"DDERR_REGIONTOOSMALL",
        L"Region passed to Clipper::GetClipList is too small"},

    {DDERR_CLIPPERISUSINGHWND, L"DDERR_CLIPPERISUSINGHWND",
        L"An attempt was made to set a clip list for a clipper objec that is "
        L"already monitoring an hwnd"},

    {DDERR_NOCLIPPERATTACHED, L"DDERR_NOCLIPPERATTACHED",
        L"No clipper object attached to surface object"},

    {DDERR_NOHWND, L"DDERR_NOHWND",
        L"Clipper notification requires an HWND or no HWND has previously "
        L"been set as the CooperativeLevel HWND"},

    {DDERR_HWNDSUBCLASSED, L"DDERR_HWNDSUBCLASSED",
        L"HWND used by DirectDraw CooperativeLevel has been subclassed, L"
        L"this prevents DirectDraw from restoring state"},

    {DDERR_HWNDALREADYSET, L"DDERR_HWNDALREADYSET",
        L"The CooperativeLevel HWND has already been set.  It can not be "
        L"reset while the process has surfaces or palettes created"},

    {DDERR_NOPALETTEATTACHED, L"DDERR_NOPALETTEATTACHED",
        L"No palette object attached to this surface"},

    {DDERR_NOPALETTEHW, L"DDERR_NOPALETTEHW",
        L"No hardware support for 16 or 256 color palettes"},

    {DDERR_BLTFASTCANTCLIP, L"DDERR_BLTFASTCANTCLIP",
        L"If a clipper object is attached to the source surface passed "
        L"into a BltFast call"},

    {DDERR_NOBLTHW, L"DDERR_NOBLTHW",
        L"No blitter hardware"},

    {DDERR_NODDROPSHW, L"DDERR_NODDROPSHW",
        L"No DirectDraw ROP hardware"},

    {DDERR_OVERLAYNOTVISIBLE, L"DDERR_OVERLAYNOTVISIBLE",
        L"GetOverlayPosition called on a hidden overlay"},

    {DDERR_NOOVERLAYDEST, L"DDERR_NOOVERLAYDEST",
        L"GetOverlayPosition called on a overlay that "
        L"UpdateOverlay has never been called on to establish a destination"},

    {DDERR_INVALIDPOSITION, L"DDERR_INVALIDPOSITION",
        L"The position of the overlay on the destination is "
        L"no longer legal for that destination"},

    {DDERR_NOTAOVERLAYSURFACE, L"DDERR_NOTAOVERLAYSURFACE",
        L"Overlay member called for a non-overlay surface"},

    {DDERR_EXCLUSIVEMODEALREADYSET, L"DDERR_EXCLUSIVEMODEALREADYSET",
        L"An attempt was made to set the cooperative level when it was "
        L"already set to exclusive"},

    {DDERR_NOTFLIPPABLE, L"DDERR_NOTFLIPPABLE",
        L"An attempt has been made to flip a surface that is not flippable"},

    {DDERR_CANTDUPLICATE, L"DDERR_CANTDUPLICATE",
        L"Can't duplicate primary & 3D surfaces, or surfaces that are "
        L"implicitly created"},

    {DDERR_NOTLOCKED, L"DDERR_NOTLOCKED",
        L"Surface was not locked.  An attempt to unlock a surface that was "
        L"not locked at all, or by this process, has been attempted"},

    {DDERR_CANTCREATEDC, L"DDERR_CANTCREATEDC",
        L"Windows can not create any more DCs"},

    {DDERR_NODC, L"DDERR_NODC",
        L"No DC was ever created for this surface"},

    {DDERR_WRONGMODE, L"DDERR_WRONGMODE",
        L"This surface can not be restored because it was created in a "
        L"different mode"},

    {DDERR_IMPLICITLYCREATED, L"DDERR_IMPLICITLYCREATED",
        L"This surface can not be restored because it is an implicitly "
        L"created surface"},

    {DDERR_NOTPALETTIZED, L"DDERR_NOTPALETTIZED",
        L"The surface being used is not a palette-based surface"},

    {DDERR_UNSUPPORTEDMODE, L"DDERR_UNSUPPORTEDMODE",
        L"The display is currently in an unsupported mode"},


    /******************/
    /*** D3D Errors ***/
    /******************/
/*

    {D3DERR_BADMAJORVERSION, L"D3DERR_BADMAJORVERSION",
        L"Bad major version"},

    {D3DERR_BADMINORVERSION, L"D3DERR_BADMINORVERSION",
        L"Bad minor version"},

    {D3DERR_EXECUTE_CREATE_FAILED, L"D3DERR_EXECUTE_CREATE_FAILED",
        L"Execute buffer create failed"},

    {D3DERR_EXECUTE_DESTROY_FAILED, L"D3DERR_EXECUTE_DESTROY_FAILED",
        L"Execute buffer destroy failed"},

    {D3DERR_EXECUTE_LOCK_FAILED, L"D3DERR_EXECUTE_LOCK_FAILED",
        L"Execute buffer lock failed"},

    {D3DERR_EXECUTE_UNLOCK_FAILED, L"D3DERR_EXECUTE_UNLOCK_FAILED",
        L"Execute buffer unlock failed"},

    {D3DERR_EXECUTE_LOCKED, L"D3DERR_EXECUTE_LOCKED",
        L"Execute buffer locked"},

    {D3DERR_EXECUTE_NOT_LOCKED, L"D3DERR_EXECUTE_NOT_LOCKED",
        L"Execute buffer not locked"},

    {D3DERR_EXECUTE_FAILED, L"D3DERR_EXECUTE_FAILED",
        L"Execute buffer execute failed"},

    {D3DERR_EXECUTE_CLIPPED_FAILED, L"D3DERR_EXECUTE_CLIPPED_FAILED",
        L"Execute buffer execute clipped failed"},

    {D3DERR_TEXTURE_NO_SUPPORT, L"D3DERR_TEXTURE_NO_SUPPORT",
        L"Texture not supported"},

    {D3DERR_TEXTURE_CREATE_FAILED, L"D3DERR_TEXTURE_CREATE_FAILED",
        L"Texture create failed"},

    {D3DERR_TEXTURE_DESTROY_FAILED, L"D3DERR_TEXTURE_DESTROY_FAILED",
        L"Texture destroy failed"},

    {D3DERR_TEXTURE_LOCK_FAILED, L"D3DERR_TEXTURE_LOCK_FAILED",
        L"Texture lock failed"},

    {D3DERR_TEXTURE_UNLOCK_FAILED, L"D3DERR_TEXTURE_UNLOCK_FAILED",
        L"Texture unlock failed"},

    {D3DERR_TEXTURE_LOAD_FAILED, L"D3DERR_TEXTURE_LOAD_FAILED",
        L"Texture load failed"},

    {D3DERR_TEXTURE_SWAP_FAILED, L"D3DERR_TEXTURE_SWAP_FAILED",
        L"Texture swap failed"},

    {D3DERR_TEXTURE_LOCKED, L"D3DERR_TEXTURE_LOCKED",
        L"Texture locked"},

    {D3DERR_TEXTURE_NOT_LOCKED, L"D3DERR_TEXTURE_NOT_LOCKED",
        L"Texture not locked"},

    {D3DERR_TEXTURE_GETSURF_FAILED, L"D3DERR_TEXTURE_GETSURF_FAILED",
        L"Texture get surface failed"},

    {D3DERR_MATRIX_CREATE_FAILED, L"D3DERR_MATRIX_CREATE_FAILED",
        L"Matrix create failed"},

    {D3DERR_MATRIX_DESTROY_FAILED, L"D3DERR_MATRIX_DESTROY_FAILED",
        L"Matrix destroy failedj"},

    {D3DERR_MATRIX_SETDATA_FAILED, L"D3DERR_MATRIX_SETDATA_FAILED",
        L"Matrix set data failed"},

    {D3DERR_MATRIX_GETDATA_FAILED, L"D3DERR_MATRIX_GETDATA_FAILED",
        L"Matrix get data failed"},

    {D3DERR_SETVIEWPORTDATA_FAILED, L"D3DERR_SETVIEWPORTDATA_FAILED",
        L"Set viewport data failed"},

    {D3DERR_MATERIAL_CREATE_FAILED, L"D3DERR_MATERIAL_CREATE_FAILED",
        L"Material create failed"},

    {D3DERR_MATERIAL_DESTROY_FAILED, L"D3DERR_MATERIAL_DESTROY_FAILED",
        L"Material destroy failed"},

    {D3DERR_MATERIAL_SETDATA_FAILED, L"D3DERR_MATERIAL_SETDATA_FAILED",
        L"Material set data failed"},

    {D3DERR_MATERIAL_GETDATA_FAILED, L"D3DERR_MATERIAL_GETDATA_FAILED",
        L"Material get data failed"},

    {D3DERR_LIGHT_SET_FAILED, L"D3DERR_LIGHT_SET_FAILED",
        L"Light set failed"},

    {D3DERR_SCENE_IN_SCENE, L"D3DERR_SCENE_IN_SCENE",
        L"Scene in scene"},

    {D3DERR_SCENE_NOT_IN_SCENE, L"D3DERR_SCENE_NOT_IN_SCENE",
        L"Scene not in scene"},

    {D3DERR_SCENE_BEGIN_FAILED, L"D3DERR_SCENE_BEGIN_FAILED",
        L"Scene begin failed"},

    {D3DERR_SCENE_END_FAILED, L"D3DERR_SCENE_END_FAILED",
        L"Scene end failed"},

*/
    /*************************************/
    /*** Direct3D Retained-Mode Errors ***/
    /*************************************/
/*
    {D3DRMERR_BADOBJECT, L"D3DRMERR_BADOBJECT",
        L"Object expected in argument"},

    {D3DRMERR_BADTYPE, L"D3DRMERR_BADTYPE",
        L"Bad argument type passed"},

    {D3DRMERR_BADALLOC, L"D3DRMERR_ALLOC",
        L"Out of memory"},

    {D3DRMERR_FACEUSED, L"D3DRMERR_FACEUSED",
        L"Face already used in a mesh"},

    {D3DRMERR_NOTFOUND, L"D3DRMERR_NOTFOUND",
        L"Object not found in specified place"},

    {D3DRMERR_NOTDONEYET, L"D3DRMERR_NOTDONEYET",
        L"Unimplemented"},

    {D3DRMERR_FILENOTFOUND, L"D3DRMERR_FILENOTFOUND",
        L"File cannot be opened"},

    {D3DRMERR_BADFILE, L"D3DRMERR_BADFILE",
        L"Data file is corrupt or has incorrect format"},

    {D3DRMERR_BADDEVICE, L"D3DRMERR_BADDEVICE",
        L"Device is not compatible with renderer"},

    {D3DRMERR_BADVALUE, L"D3DRMERR_BADVALUE",
        L"Bad argument value passed"},

    {D3DRMERR_BADMAJORVERSION, L"D3DRMERR_BADMAJORVERSION",
        L"Bad DLL major version"},

    {D3DRMERR_BADMINORVERSION, L"D3DRMERR_BADMINORVERSION",
        L"Bad DLL minor version"},

    {D3DRMERR_UNABLETOEXECUTE, L"D3DRMERR_UNABLETOEXECUTE",
        L"Unable to carry out procedure"},*/

    //
    // DirectInput errors
    //

    { DI_BUFFEROVERFLOW, L"DI_BUFFEROVERFLOW", L"The device buffer overflowed and some input was lost. This value is equal to the S_FALSE standard COM return value."},
    { DI_DOWNLOADSKIPPED , L"DI_DOWNLOADSKIPPED ",L"The parameters of the effect were successfully updated, but the effect could not be downloaded because the associated device was not acquired in exclusive mode."},
    { DI_EFFECTRESTARTED , L"DI_EFFECTRESTARTED ",L"The effect was stopped, the parameters were updated, and the effect was restarted."},
    { DI_NOEFFECT , L"DI_NOEFFECT ",L"The operation had no effect. This value is equal to the S_FALSE standard COM return value."},
    { DI_NOTATTACHED , L"DI_NOTATTACHED ",L"The device exists but is not currently attached. This value is equal to the S_FALSE standard COM return value."},
    { DI_OK , L"DI_OK ",L"The operation completed successfully. This value is equal to the S_OK standard COM return value."},
    { DI_POLLEDDEVICE , L"DI_POLLEDDEVICE ",L"The device is a polled device. As a result, device buffering will not collect any data and event notifications will not be signaled until the IDirectInputDevice2::Poll method is called."},
    { DI_PROPNOEFFECT , L"DI_PROPNOEFFECT ",L"The change in device properties had no effect. This value is equal to the S_FALSE standard COM return value."},
    { DI_TRUNCATED , L"DI_TRUNCATED ",L"The parameters of the effect were successfully updated, but some of them were beyond the capabilities of the device and were truncated to the nearest supported value."},
    { DI_TRUNCATEDANDRESTARTED , L"DI_TRUNCATEDANDRESTARTED ",L"Equal to DI_EFFECTRESTARTED | DI_TRUNCATED."},
    { DIERR_ACQUIRED , L"DIERR_ACQUIRED ",L"The operation cannot be performed while the device is acquired."},
    { DIERR_ALREADYINITIALIZED , L"DIERR_ALREADYINITIALIZED ",L"This object is already initialized"},
    { DIERR_BADDRIVERVER , L"DIERR_BADDRIVERVER ",L"The object could not be created due to an incompatible driver version or mismatched or incomplete driver components."},
    { DIERR_BETADIRECTINPUTVERSION , L"DIERR_BETADIRECTINPUTVERSION ",L"The application was written for an unsupported prerelease version of DirectInput."},
    { DIERR_DEVICEFULL , L"DIERR_DEVICEFULL ",L"The device is full."},
    { DIERR_DEVICENOTREG , L"DIERR_DEVICENOTREG ",L"The device or device instance is not registered with DirectInput. This value is equal to the REGDB_E_CLASSNOTREG standard COM return value."},
    { DIERR_EFFECTPLAYING , L"DIERR_EFFECTPLAYING ",L"The parameters were updated in memory but were not downloaded to the device because the device does not support updating an effect while it is still playing."},
    { DIERR_HASEFFECTS , L"DIERR_HASEFFECTS ",L"The device cannot be reinitialized because there are still effects attached to it."},
    { DIERR_GENERIC , L"DIERR_GENERIC ",L"An undetermined error occurred inside the DirectInput subsystem. This value is equal to the E_FAIL standard COM return value."},
    { DIERR_HANDLEEXISTS , L"DIERR_HANDLEEXISTS ",L"The device already has an event notification associated with it. This value is equal to the E_ACCESSDENIED standard COM return value."},
    { DIERR_INCOMPLETEEFFECT , L"DIERR_INCOMPLETEEFFECT ",L"The effect could not be downloaded because essential information is missing. For example, no axes have been associated with the effect, or no type-specific information has been supplied."},
    { DIERR_INPUTLOST , L"DIERR_INPUTLOST ",L"Access to the input device has been lost. It must be reacquired."},
    { DIERR_INVALIDPARAM , L"DIERR_INVALIDPARAM ",L"An invalid parameter was passed to the returning function, or the object was not in a state that permitted the function to be called."},
    { DIERR_MOREDATA , L"DIERR_MOREDATA ",L"Not all the requested information fitted into the buffer."},
    { DIERR_NOAGGREGATION , L"DIERR_NOAGGREGATION ",L"This object does not support aggregation."},
    { DIERR_NOINTERFACE , L"DIERR_NOINTERFACE ",L"The specified interface is not supported by the object. This value is equal to the E_NOINTERFACE standard COM return value."},
    { DIERR_NOTACQUIRED , L"DIERR_NOTACQUIRED ",L"The operation cannot be performed unless the device is acquired."},
    { DIERR_NOTBUFFERED , L"DIERR_NOTBUFFERED ",L"The device is not buffered. Set the DIPROP_BUFFERSIZE property to enable buffering."},
    { DIERR_NOTDOWNLOADED , L"DIERR_NOTDOWNLOADED ",L"The effect is not downloaded."},
    { DIERR_NOTEXCLUSIVEACQUIRED , L"DIERR_NOTEXCLUSIVEACQUIRED ",L"The operation cannot be performed unless the device is acquired in DISCL_EXCLUSIVE mode."},
    { DIERR_NOTFOUND , L"DIERR_NOTFOUND ",L"The requested object does not exist."},
    { DIERR_NOTINITIALIZED , L"DIERR_NOTINITIALIZED ",L"This object has not been initialized."},
    { DIERR_OBJECTNOTFOUND , L"DIERR_OBJECTNOTFOUND ",L"The requested object does not exist."},
    { DIERR_OLDDIRECTINPUTVERSION , L"DIERR_OLDDIRECTINPUTVERSION ", L"he application requires a newer version of DirectInput."},
    { DIERR_OTHERAPPHASPRIO , L"DIERR_OTHERAPPHASPRIO ",L"Another application has a higher priority level, preventing this call from succeeding."},
    { DIERR_OUTOFMEMORY , L"DIERR_OUTOFMEMORY ",L"The DirectInput subsystem couldn't allocate sufficient memory to complete the call. This value is equal to the E_OUTOFMEMORY standard COM return value."},
    { DIERR_READONLY , L"DIERR_READONLY ",L"The specified property cannot be changed. This value is equal to the E_ACCESSDENIED standard COM return value."},
    { DIERR_UNSUPPORTED , L"DIERR_UNSUPPORTED ",L"The function called is not supported at this time. This value is equal to the E_NOTIMPL standard COM return value."},

    /************************************/
    /*** Miscellaneous Windows Errors ***/
    /************************************/

    {ERROR_INVALID_PARAMETER, L"ERROR_INVALID_PARAMETER",
        L"Invalid parameter"},

    {ERROR_NOT_ENOUGH_MEMORY, L"ERROR_NOT_ENOUGH_MEMORY",
        L"Insufficient memory available"},

    {ERROR_OUTOFMEMORY, L"ERROR_OUTOFMEMORY",
        L"Out of memory"},

    {0,0,0}
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

bool g_bDumpDD = false;

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

bool DDError(HRESULT hresult, const char* pszCall, const wchar_t* pszFile, int line, const wchar_t* pszModule)
{
    if (g_bDumpDD) {
        ZDebugOutput(ZString(pszCall) + L"\n");
    }

    if (FAILED(hresult)) {
        HRESULTInfo* pinfo = gpinfo;

        while (pinfo->hresult != hresult && pinfo->hresult != 0) {
            pinfo++;
        }

        ZDebugOutput(ZString(pszCall) + L"\n");
        if (pinfo->hresult == 0) {
            ZAssertImpl(false, L"unknown error code", pszFile, line, pszModule);
        } else {
            ZAssertImpl(false, pinfo->pszText, pszFile, line, pszModule);
        }
        
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

bool DDSCallImpl(
    HRESULT hr,
	const char* pszCall,
	const wchar_t* pszFile,
    int line,
	const wchar_t* pszModule
) {
    if (
           hr == DDERR_SURFACELOST
        || hr == DDERR_SURFACEBUSY
        || hr == DDERR_UNSUPPORTED
    ) {
        return false;
    }

    return DDError(hr, pszCall, pszFile, line, pszModule);
}

#endif // _DEBUG
