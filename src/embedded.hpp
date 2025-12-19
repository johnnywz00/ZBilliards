#ifndef EMBEDDED_HPP
#define EMBEDDED_HPP

#ifdef _EMBEDDING

#include "embedded_data/BigBook-Heavy.hpp"
#include "embedded_data/HelveticaNeue.hpp"
#include "embedded_data/NE.hpp"
#include "embedded_data/NW.hpp"
#include "embedded_data/SE.hpp"
#include "embedded_data/SW.hpp"
#include "embedded_data/arrowicon.hpp"
#include "embedded_data/ball.hpp"
#include "embedded_data/cue.hpp"
#include "embedded_data/cueball.hpp"
#include "embedded_data/cueshot.hpp"
#include "embedded_data/hurt2.hpp"
#include "embedded_data/icon.hpp"
#include "embedded_data/pocket.hpp"
#include "embedded_data/railbump.hpp"
#include "embedded_data/rockHit2.hpp"
#include "embedded_data/tablecorner.hpp"

inline unordered_map<string, unsigned char*> idMap {
{ "BigBook-Heavy.otf",		__BigBook_Heavy_otf },
{ "HelveticaNeue.ttc",		__HelveticaNeue_ttc },
{ "NE.png",		__NE_png },
{ "NW.png",		__NW_png },
{ "SE.png",		__SE_png },
{ "SW.png",		__SW_png },
{ "arrowicon.png",		__arrowicon_png },
{ "ball.png",		__ball_png },
{ "cue.png",		__cue_png },
{ "cueball.png",		__cueball_png },
{ "cueshot.wav",		__cueshot_wav },
{ "hurt2.wav",		__hurt2_wav },
{ "icon.png",		__icon_png },
{ "pocket.png",		__pocket_png },
{ "railbump.wav",		__railbump_wav },
{ "rockHit2.wav",		__rockHit2_wav },
{ "tablecorner.png",		__tablecorner_png }
};

inline unordered_map<string, unsigned int> lenMap {
{ "BigBook-Heavy.otf",		__BigBook_Heavy_otf_len },
{ "HelveticaNeue.ttc",		__HelveticaNeue_ttc_len },
{ "NE.png",		__NE_png_len },
{ "NW.png",		__NW_png_len },
{ "SE.png",		__SE_png_len },
{ "SW.png",		__SW_png_len },
{ "arrowicon.png",		__arrowicon_png_len },
{ "ball.png",		__ball_png_len },
{ "cue.png",		__cue_png_len },
{ "cueball.png",		__cueball_png_len },
{ "cueshot.wav",		__cueshot_wav_len },
{ "hurt2.wav",		__hurt2_wav_len },
{ "icon.png",		__icon_png_len },
{ "pocket.png",		__pocket_png_len },
{ "railbump.wav",		__railbump_wav_len },
{ "rockHit2.wav",		__rockHit2_wav_len },
{ "tablecorner.png",		__tablecorner_png_len }
};

#endif


inline bool loadByMethod (Texture& t, string fname) {
#ifdef _EMBEDDING
    size_t lastSep = fname.find_last_of("/");
    fname = (lastSep == string::npos) ? fname : fname.substr(lastSep + 1);
    return t.loadFromMemory(idMap[fname], lenMap[fname]);
#else
    return t.loadFromFile(fname);
#endif
}

inline bool loadByMethod (SoundBuffer& t, string fname) {
#ifdef _EMBEDDING
    size_t lastSep = fname.find_last_of("/");
    fname = (lastSep == string::npos) ? fname : fname.substr(lastSep + 1);
    return t.loadFromMemory(idMap[fname], lenMap[fname]);
#else
    return t.loadFromFile(fname);
#endif
}

inline bool loadByMethod (Font& t, string fname) {
#ifdef _EMBEDDING
    size_t lastSep = fname.find_last_of("/");
    fname = (lastSep == string::npos) ? fname : fname.substr(lastSep + 1);
    return t.loadFromMemory(idMap[fname], lenMap[fname]);
#else
    return t.loadFromFile(fname);
#endif
}

inline bool loadByMethod (Image& t, string fname) {
#ifdef _EMBEDDING
    size_t lastSep = fname.find_last_of("/");
    fname = (lastSep == string::npos) ? fname : fname.substr(lastSep + 1);
    return t.loadFromMemory(idMap[fname], lenMap[fname]);
#else
    return t.loadFromFile(fname);
#endif
}

#endif
