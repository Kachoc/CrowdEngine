//
// Exceptions.h
//
// D�finit les classes d'exceptions utilis�es par le moteur et utilisables par les projets d�pendant du moteur
//

#ifndef CE_EXCEPTIONS_H_INCLUDED
#define CE_EXCEPTIONS_H_INCLUDED

#include <exception>
#include "CEString.h"
#include "Export.h"

namespace Crowd
{

class Exception : public std::exception
{
	// Classe de base de laquelle d�rivent les autres classes d'exception
	// D�rive de std::exception

	public:
	// Constructeurs, destructeur
	CRWD_EXPORT Exception(const String &msg = "") throw();
	CRWD_EXPORT Exception(const Exception &) throw();
	
	CRWD_EXPORT virtual ~Exception() throw();


	// retourne un message d�crivant l'erreur
	CRWD_EXPORT virtual const char *what() const throw();

	// permet d'ajouter msg au d�but du message d'erreur contenu par l'exception (pour les rethrow, notamment)
	CRWD_EXPORT virtual void addFront(const String &msg);

	// permet d'ajouter msg � la fin du message d'erreur contenu par l'exception (pour les rethrow, notamment)
	CRWD_EXPORT virtual void addBack(const String &msg);


	// permet de d�finir le message d'erreur contenu par l'exception
	CRWD_EXPORT virtual void setString(const String &msg);


	// copie l'exception (pour le multithreading)
	// doit �tre red�finie par les classes d�riv�es pour conserver le type de l'exception (il est possible d'utiliser la macro EXCEPTION_COPY(EXCEPTION) d�finie ci-dessous)
	CRWD_EXPORT virtual Exception *copy();

	protected:
	// d�finit le message d'erreur contenu par l'exception, fonction utilis�e seulement pour le multithreading
	CRWD_EXPORT virtual void setMsg(const String &msg);

	protected:
	// message d'erreur
	String m_msg;
};

// Macro qui permet de g�n�rer automatiquement la fonction membre Exception *Copy(void) pour les classes d�riv�es de Crowd::Exception.
// Cette fonction est n�cessaire pour la gestion des exceptions entre les threads.
// La classe d�riv�e de Crowd::Exception doit avoir un constructeur par d�faut du type EXCEPTION() pour que cette macro fonctionne.
#define EXCEPTION_COPY(EXCEPTION) public: virtual Exception *copy() { EXCEPTION *e = new EXCEPTION(*this); return e; }


////
////
//// Classes d�riv�es
////
////

class LoadFailure : public Exception
{
	public:
	CRWD_EXPORT LoadFailure(const String &msg = "") throw();
	CRWD_EXPORT LoadFailure(const LoadFailure &) throw();

	CRWD_EXPORT virtual ~LoadFailure() throw();

	EXCEPTION_COPY(LoadFailure);
};

class InitFailure : public Exception
{
	public:
	CRWD_EXPORT InitFailure(const String &error = "", const String &init = "") throw();
	// donner le nom de l'initialisation (ex: "Direct 3D 10") et l'erreur (ex: "Il faut cr�er une fen�tre avant.")
	CRWD_EXPORT InitFailure(const InitFailure &) throw();

	CRWD_EXPORT virtual ~InitFailure() throw();

	EXCEPTION_COPY(InitFailure);
};

class WCompatibility : public Exception
{
	public:
	CRWD_EXPORT WCompatibility(const String &error = "") throw();

	CRWD_EXPORT virtual ~WCompatibility() throw();

	EXCEPTION_COPY(WCompatibility);
};

class OutOfMemory : public Exception
{
	public:
	CRWD_EXPORT OutOfMemory() throw();
	CRWD_EXPORT virtual ~OutOfMemory() throw();

	EXCEPTION_COPY(OutOfMemory);
};

class Error : public Exception
{
	public:
	CRWD_EXPORT Error(const String &error = "", const String &type = "ind�fini");
	CRWD_EXPORT virtual ~Error();

	EXCEPTION_COPY(Error);
};

class WMemoryAccess : public Exception
{
	public:
	CRWD_EXPORT WMemoryAccess(const String &msg = "");

	EXCEPTION_COPY(WMemoryAccess);
};

class RequestConditions : public Exception // conditions non remplies pour performer une action
{
	public:
	CRWD_EXPORT RequestConditions(const String &what = "", const String &conditions = "");

	EXCEPTION_COPY(RequestConditions);
};

class WParameters : public Exception // Wrong parameters
{
	public:
	CRWD_EXPORT WParameters(const String &function = "", const String &parameters = "");

	EXCEPTION_COPY(WParameters);
};

} // namespace Crowd

#endif // CE_EXCEPTIONS_H_INCLUDED