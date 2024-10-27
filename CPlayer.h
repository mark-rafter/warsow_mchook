#ifndef _CPLAYER_HEADER 
#define _CPLAYER_HEADER

#define BONE_TABLE_SIZE 5
#define NUM_HISTORY_FRAMES 256

typedef enum
{
	BOX_STAND,
	BOX_CROUCH,
	BOX_ALL
} Stance_t;

class CPlayer
{
	public:
		CPlayer( );
		CPlayer( centity_t * );
		~CPlayer( );

		//	Functions
		void			Clear( void );

		void			Draw_2D( void );
		virtual void	DrawLight( void );

		virtual centity_t*GetClientEnt( void );
		virtual int		GetClientNum( void );
		virtual float	GetDistance( void );
		virtual float	*GetLerpAngles( void );
		inline float	*GetLerpOrigin( void );
		virtual char	*GetName( void );
		virtual int		GetPing( void );
		inline int		GetTeam( void );
		virtual float	*GetVelocity( void );
		virtual float	*GetVisibleOrigin( bool );

		virtual bool	IsEnemy( void );
		virtual bool	IsVisible( void );
		virtual bool	IsDead( void );

		void			UpdateBones( void );
		inline void		UpdateDistance( void );
		inline void		UpdatePing( void );
		void			UpdatePrediction( void );
		void			UpdateTeam( void );
		void			UpdateVectors( void );
		virtual void	UpdatePlayerState( void );

		int				iHealth;
		bool			bValid, bDraw2D, bUpdated;

	private:
		bool			m_bEnemy, m_bMe;
		bool			m_bVisible, m_bDead;
		vec3_t			m_fBoneOrigin[37];	// TODO: NOTE in future versions, aimbot will f'up
		vec4_t			m_fColor;			// if this array is too small for the skel in use
		float			m_fDistance;
		vec3_t			m_fLerpAngles, m_fLerpOrigin;
		int				m_iClientNum;
		int				m_iVisibleBone;
		int				m_iBone, m_iNumAllBones;
		std::vector<int>m_vBoneTable;
		centity_t		*m_pClientEnt;
		int				m_iPing;
		vec3_t			m_fHitboxOrigin[7]; // TODO: expand the number of these, for now, just down the center
		int				m_iVisibleHitbox;
		vec3_t			m_fHitboxMins[BOX_ALL], m_fHitboxMaxs[BOX_ALL];

		vec3_t			m_fVelocity;
		int				m_iHistoryIndex;
		vec3_t			m_fOriginHistory[NUM_HISTORY_FRAMES];
		int				m_iTimeHistory[NUM_HISTORY_FRAMES];
};

extern CPlayer vPlayers[NUM_MAX_CLIENTS];
extern CPlayer *pAimbotTarget, *pMe, *pSpycamClient;
extern void UpdatePlayers( centity_t * );

/*
*** Viciious ***

Bone[ 4]: bip01 spine2
Bone[ 7]: bip01 l clavicle
Bone[13]: bip01 r clavicle
Bone[ 5]: bip01 neck
Bone[ 6]: bip01 head
Bone[ 3]: bip01 spine1
Bone[ 2]: bip01 spine
Bone[ 0]: bip01
Bone[ 1]: bip01 pelvis
Bone[ 8]: bip01 l upperarm
Bone[14]: bip01 r upperarm

Bone[19]: bip01 l thigh
Bone[23]: bip01 r thigh
Bone[20]: bip01 l calf
Bone[24]: bip01 r calf

*** Bobot ***

Bone[ 8]: bip01 spine2
Bone[12]: bip01 l clavicle
Bone[20]: bip01 r clavicle
Bone[ 9]: bip01 neck
Bone[10]: bip01 head
Bone[ 7]: bip01 spine1
Bone[ 6]: bip01 spine
Bone[ 3]: bip01
Bone[ 5]: bip01 pelvis
Bone[13]: bip01 l upperarm
Bone[21]: bip01 r upperarm

Bone[28]: bip01 l thigh
Bone[33]: bip01 r thigh
Bone[29]: bip01 l calf
Bone[34]: bip01 r calf
Bone[11]: bip01 headnub

*** Monada ***

Bone[ 4]: bip01 spine2
Bone[ 7]: bip01 l clavicle
Bone[13]: bip01 r clavicle
Bone[ 5]: bip01 neck
Bone[ 6]: bip01 head
Bone[ 3]: bip01 spine1
Bone[ 2]: bip01 spine
Bone[ 0]: bip01
Bone[ 1]: bip01 pelvis
Bone[ 8]: bip01 l upperarm
Bone[14]: bip01 r upperarm

Bone[19]: bip01 l thigh
Bone[23]: bip01 r thigh
Bone[20]: bip01 l calf
Bone[24]: bip01 r calf

*** Padpork ***

Bone[ 5]: bip01 spine3
Bone[ 4]: bip01 spine2
Bone[12]: bip01 l clavicle
Bone[19]: bip01 r clavicle
Bone[ 6]: bip01 neck
Bone[ 7]: bip01 head
Bone[ 3]: bip01 spine1
Bone[ 2]: bip01 spine
Bone[ 0]: bip01
Bone[ 1]: bip01 pelvis
Bone[13]: bip01 l upperarm
Bone[20]: bip01 r upperarm

Bone[26]: bip01 l thigh
Bone[30]: bip01 r thigh
Bone[27]: bip01 l calf
Bone[31]: bip01 r calf

Bone[ 8]: bip01 ponytail1
Bone[ 9]: bip01 ponytail11
Bone[10]: bip01 ponytail2
Bone[11]: bip01 ponytail21

*** Silverclaw ***

Bone[ 4]: bip01 spine2
Bone[ 7]: bip01 l clavicle
Bone[13]: bip01 r clavicle
Bone[ 5]: bip01 neck
Bone[ 6]: bip01 head
Bone[ 3]: bip01 spine1
Bone[ 2]: bip01 spine
Bone[ 0]: bip01
Bone[ 1]: bip01 pelvis
Bone[ 8]: bip01 l upperarm
Bone[14]: bip01 r upperarm

Bone[19]: bip01 l thigh
Bone[23]: bip01 r thigh
Bone[20]: bip01 l calf
Bone[24]: bip01 r calf
*/

#endif // _CPLAYER_HEADER