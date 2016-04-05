/*
 * =====================================================================================
 *
 *       Filename:  horo_AudioSpatializer.h
 *
 *    Description:  speaker layout spatializing experiments
 *
 *        Version:  1.0
 *        Created:  02/26/2014 16:06:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:
 *
 * =====================================================================================
 */


#ifndef MY_AUDIO_SPATIALIZER_INCLUDED
#define MY_AUDIO_SPATIALIZER_INCLUDED

#include "gfx/gfx_matrix.h"
#include <vector>

using gfx::Vec3f;
using gfx::Quat;


#define DESKTOP_LAYOUT 0
#define NUM_DESKTOP_SPEAKERS 2

#define ALLO_LAYOUT 1
#define NUM_ALLO_SPEAKERS 60


namespace hs {

  /// Layout of Spatialized Speakers, templated on LAYOUT and NUMBER
  template<int L, int N >
  struct SpeakerLayout {

  	SpeakerLayout(float r = 3) { calcRolloff(r); init(); }

    /// Initialize Geometry
    void init();

    /// A Group of channels
    struct Channel {
      vector<int> idx;
    };

    static vector<Channel> Group;

    /// Positions of Speakers
    gfx::Vec3f pos[N];
    float RollOff;



    /// Calculate DBAP at v
  	vector<float>  mix( gfx::Vec3f v );
    /// Calculate ROLLOFF
  	void calcRolloff( float dec) { RollOff = pow(10., -dec/20.0); }

    /// Number of Channels
  	int num() { return N; }

    /// Move by n channels
  	int move( int ch, int n ){
  		return (ch + n) % N;
  	  }

    /// Select opposite channel
  	int opp( int ch ) { return move ( ch, N / 2. ); }
    /// Select to next channel
  	int next( int ch ){ return move (ch, ch + 1); }
    /// Select reflected channel
  	int reflect( int ch)  { return ref[ch]; }
  	int ref[N];

  };

	template<int L, int N>
	inline vector<float> SpeakerLayout<L,N> :: mix  (gfx::Vec3f v) {

		vector<float> m;

		float sum;
		for (int i = 0; i < N; ++i){
			gfx::Vec3f dir = pos[i] - v;

			float mag = dir.sq();
			float id = 0;
			float dist;
			if (!mag == 0 ){
				id =1.0 / mag;
				dist = 1.0/sqrt(mag);
			}
			m.push_back( dist );
			sum += id;
		}

		float k = RollOff / sqrt(sum);

		for (int i = 0; i < N; ++i){
			m[i] *= k;
		}

		return m;

	}

  /// Desktop Speaker Layout
	template<int L>
	inline void Speakers<L,2> :: init(){

    cout << "INIT DESKTOP SPEAKER LAYOUT" << endl;

		pos[0] = gfx::Vec3f(-1,0,0);
		pos[1] = gfx::Vec3f(1,0,0);
	}

  /// ALLOSPHERE SPEAKER LAYOUT note: subwoofer is [47]
	template<>
	inline void SpeakerLayout<ALLO_SPEAKER_LAYOUT> :: init()   {

    cout << "INIT ALLOSPHERE SPEAKER LAYOUT" << endl;

		gfx::Vec3f mv(-1,0,0);

		//1 - 12 top ring
	  int ix = 0;
		gfx::Vec3f tv = gfx::Quat::spin( mv, gfx::Quat( PIOVERFOUR / 2.0, gfx::Vec3f(0,0,-1) ) );
		//cout << "TOP " << tv << endl;
		 for (int i = 0; i < 12; ++i){
			float t = 1.0 * ix / 12.0;
			gfx::Quat q( PI * t, gfx::Vec3f(0,1,0));
			pos[i] = gfx::Quat::spin( tv, q );
			//cout << i << " " << pos[i] << endl;
			ix++;
		}


		//17 - 46 middle ring
	  ix = 0;
		//cout << "middle " << mv << endl;
		for (int i = 16; i < 46; ++i){
			float t = 1.0 * ix / 20.0;
			gfx::Quat q( PI * t, gfx::Vec3f(0,1,0));
			pos[i] = gfx::Quat::spin( mv, q );
			ix++;
		}

		//49-60 bottom ring
		ix = 0;
		gfx::Vec3f bv = gfx::Quat::spin( mv, gfx::Quat( -PIOVERFOUR/2.0, gfx::Vec3f(0,0,-1) ) );
		//cout << "bottom " << bv << endl;
		for (int i = 48; i < 60; ++i){
			double t = 1.0 * ix / 12.0;
			gfx::Quat q( PI * t, gfx::Vec3f(0,1,0));
			pos[i] = gfx::Quat::spin( bv, q );
			ix++;
		}

	}


  template<int N>
  struct AudioSource;

  template< int N >
  ostream& operator << (ostream& os, SpatialSource<N>& s);

  /// Audio Source
  template< int N >
  struct AudioSource {

    vector<float> f;
  	Speakers< N > speakers;
  	gfx::Vec3f pos;

  	int num() { return N; }

  	void operator() (){
  		f = speakers.mix( pos );
  	}

  	float operator[] (int idx) const{
  		return f[idx];
  	}

  	template<int S>
  	friend ostream& operator << ( ostream& os, SpatialSource<S> &s);
  };

  template<int N>
  inline ostream& operator << ( ostream& os, SpatialSource<N> & s){
  	for (int i = 0; i < s.speakers.num(); ++i ){
  		os << "mix at: " << i << " is " << s[i] << " \n";
  	}
  	return os;
  }

#endif
