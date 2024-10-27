#ifndef _CAIMBOT_HEADER 
#define _CAIMBOT_HEADER

typedef struct
{
	char szName[20];
	float fProjSpeed;
	unsigned int iTimeout;
} WeapInfo_t;

class CAimbot
{
public:
	CAimbot( );
	~CAimbot( );

	//	Functions
	void			Aim( bool );
	void			AutoShoot( usercmd_t * );
	bool			CalcWeapon( vec3_t );
	bool			ChangeWeapon( int, float );
	bool			CheckTargetState( CPlayer * );
	void			FindTarget( CPlayer * );

	virtual bool	IsAiming( void );

	bool			PlayerInFov( CPlayer *, int );
	void			PredictProjectileShot( float, vec3_t, vec3_t );

	virtual void	UnAim( void );
	
	//	Vars
	float			fOldSens;
	int				iPointedPlayer;

private:
	//	Vars
	bool			m_bAiming, m_bShooting;
};

extern CAimbot Aimbot;

#endif // _CAIMBOT_HEADER