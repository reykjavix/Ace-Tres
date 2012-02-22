// fstream standard header
#pragma once
#ifndef _FSTREAM_
#define _FSTREAM_
#ifndef RC_INVOKED
#include <istream>

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)

_STD_BEGIN
 #pragma warning(disable: 4127)

extern _MRTIMP2_NCEEPURE _Filet *__CLRCALL_PURE_OR_CDECL _Fiopen(
	const char *,
	ios_base::openmode, int);
extern _MRTIMP2_NCEEPURE _Filet *__CLRCALL_PURE_OR_CDECL _Fiopen(
	const wchar_t *,
	ios_base::openmode, int);

 #ifdef _NATIVE_WCHAR_T_DEFINED
extern _MRTIMP2_NCEEPURE _Filet *__CLRCALL_PURE_OR_CDECL _Fiopen(
	const unsigned short *,
	ios_base::openmode, int);
 #endif /* _NATIVE_WCHAR_T_DEFINED */

		// TEMPLATE FUNCTION _Fgetc
template<class _Elem> inline
	bool _Fgetc(_Elem& _Ch, _Filet *_File)
	{	// get an element from a C stream
	return (fread(&_Ch, sizeof (_Elem), 1, _File) == 1);
	}

template<> inline bool _Fgetc(char& _Byte, _Filet *_File)
	{	// get a char element from a C stream
	int _Meta;
	if ((_Meta = fgetc(_File)) == EOF)
		return (false);
	else
		{	// got one, convert to char
		_Byte = (char)_Meta;
		return (true);
		}
	}

template<> inline bool _Fgetc(wchar_t& _Wchar, _Filet *_File)
	{	// get a wchar_t element from a C stream
	wint_t _Meta;
	if ((_Meta = _CSTD fgetwc(_File)) == WEOF)
		return (false);
	else
		{	// got one, convert to wchar_t
		_Wchar = (wchar_t)_Meta;
		return (true);
		}
	}

 #ifdef _NATIVE_WCHAR_T_DEFINED
template<> inline bool _Fgetc(unsigned short& _Wchar, _Filet *_File)
	{	// get an unsigned short element from a C stream
	wint_t _Meta;
	if ((_Meta = _CSTD fgetwc(_File)) == WEOF)
		return (false);
	else
		{	// got one, convert to unsigned short
		_Wchar = (unsigned short)_Meta;
		return (true);
		}
	}
 #endif /* _NATIVE_WCHAR_T_DEFINED */

		// TEMPLATE FUNCTION _Fputc
template<class _Elem> inline
	bool _Fputc(_Elem _Ch, _Filet *_File)
	{	// put an element to a C stream
	return (fwrite(&_Ch, 1, sizeof (_Elem), _File) == sizeof (_Elem));
	}

template<> inline bool _Fputc(char _Byte, _Filet *_File)
	{	// put a char element to a C stream
	return (fputc(_Byte, _File) != EOF);
	}

template<> inline bool _Fputc(wchar_t _Wchar, _Filet *_File)
	{	// put a wchar_t element to a C stream
	return (_CSTD fputwc(_Wchar, _File) != WEOF);
	}

 #ifdef _NATIVE_WCHAR_T_DEFINED
template<> inline bool _Fputc(unsigned short _Wchar, _Filet *_File)
	{	// put an unsigned short element to a C stream
	return (_CSTD fputwc(_Wchar, _File) != WEOF);
	}
 #endif /* _NATIVE_WCHAR_T_DEFINED */

		// TEMPLATE FUNCTION _Ungetc
template<class _Elem> inline
	bool _Ungetc(const _Elem& _Ch, _Filet *_File)
	{	// put back an arbitrary element to a C stream (always fail)
	return (false);
	}

template<> inline bool _Ungetc(const char& _Byte, _Filet *_File)
	{	// put back a char element to a C stream
	return (ungetc((unsigned char)_Byte, _File) != EOF);
	}

template<> inline bool _Ungetc(const signed char& _Byte, _Filet *_File)
	{	// put back a signed char element to a C stream
	return (ungetc((unsigned char)_Byte, _File) != EOF);
	}

template<> inline bool _Ungetc(const unsigned char& _Byte, _Filet *_File)
	{	// put back an unsigned char element to a C stream
	return (ungetc(_Byte, _File) != EOF);
	}

template<> inline bool _Ungetc(const wchar_t& _Wchar, _Filet *_File)
	{	// put back a wchar_t element to a C stream
	return (_CSTD ungetwc(_Wchar, _File) != WEOF);
	}

 #ifdef _NATIVE_WCHAR_T_DEFINED
template<> inline bool _Ungetc(const unsigned short& _Wchar, _Filet *_File)
	{	// put back an unsigned short element to a C stream
	return (_CSTD ungetwc(_Wchar, _File) != WEOF);
	}
 #endif /* _NATIVE_WCHAR_T_DEFINED */

		// TEMPLATE CLASS basic_filebuf
template<class _Elem,
	class _Traits>
	class basic_filebuf
		: public basic_streambuf<_Elem, _Traits>
	{	// stream buffer associated with a C stream
public:
	typedef basic_filebuf<_Elem, _Traits> _Myt;
	typedef basic_streambuf<_Elem, _Traits> _Mysb;
	typedef typename _Traits::state_type _Myst;
	typedef codecvt<_Elem, char, typename _Traits::state_type> _Cvt;

	virtual __CLR_OR_THIS_CALL ~basic_filebuf()
		{	// destroy the object
		if (_Myfile != 0)
			_Reset_back();	// revert from _Mychar buffer
		if (_Closef)
			close();
		}

	basic_filebuf(_Filet *_File = 0)
		: _Mysb()
		{	// construct from pointer to C stream
		_Init(_File, _Newfl);
		}

	typedef typename _Traits::int_type int_type;
	typedef typename _Traits::pos_type pos_type;
	typedef typename _Traits::off_type off_type;

	basic_filebuf(_Uninitialized)
		: _Mysb(_Noinit)
		{	// construct uninitialized
		}

	basic_filebuf(_Myt&& _Right)
		{	// construct by copying _Right
		_Init(_Right._Myfile, _Newfl);	// match buffering styles
		_Init((_Filet *)0, _Closefl);	// then make *this look closed
		_Assign_rv(_STD forward<_Myt>(_Right));
		}

	_Myt& operator=(_Myt&& _Right)
		{	// assign from _Right
		_Assign_rv(_STD forward<_Myt>(_Right));
		return (*this);
		}

	void _Assign_rv(_Myt&& _Right)
		{	// assign by moving _Right
		if (this != &_Right)
			{	// different, worth moving
			close();
			this->swap(_Right);
			}
		}

	void swap(_Myt& _Right)
		{	// swap with _Right
		if (this != &_Right)
			{	// different, worth swapping
			_Mysb::swap(_Right);
			_STD swap(_Pcvt, _Right._Pcvt);
			_STD swap(_Mychar, _Right._Mychar);
			_STD swap(_Wrotesome, _Right._Wrotesome);
			_STD swap(_State, _Right._State);
			_STD swap(_Closef, _Right._Closef);
			_STD swap(_Myfile, _Right._Myfile);
			}
		}

	void swap(_Myt&& _Right)
		{	// swap with _Right
		_Assign_rv(_STD forward<_Myt>(_Right));
		}

	enum _Initfl
		{	// reasons for a call to _Init
		_Newfl, _Openfl, _Closefl};

	bool is_open() const
		{	// test if C stream has been opened
		return (_Myfile != 0);
		}

	_Myt *open(const char *_Filename,
		ios_base::openmode _Mode,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a C stream with specified mode
		_Filet *_File;
		if (_Myfile != 0 || (_File = _Fiopen(_Filename, _Mode, _Prot)) == 0)
			return (0);	// open failed

		_Init(_File, _Openfl);
		_Initcvt(&_USE(_Mysb::getloc(), _Cvt));
		return (this);	// open succeeded
		}

 #if _HAS_CPP0X
	_Myt *open(const string& _Str,
		ios_base::openmode _Mode,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a C stream with specified mode
		return (open(_Str.c_str(), _Mode, _Prot));
		}
 #endif /* _HAS_CPP0X */

	_Myt *open(const char *_Filename, ios_base::open_mode _Mode)
		{	// open a C stream with specified mode (old style)
		return (open(_Filename, (ios_base::openmode)_Mode));
		}

	_Myt *open(const wchar_t *_Filename,
		ios_base::openmode _Mode,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a wide-named C stream -- EXTENSION
		_Filet *_File;
		if (_Myfile != 0 || (_File = _Fiopen(_Filename, _Mode, _Prot)) == 0)
			return (0);	// open failed

		_Init(_File, _Openfl);
		_Initcvt(&_USE(_Mysb::getloc(), _Cvt));
		return (this);	// open succeeded
		}

 #if _HAS_CPP0X
	_Myt *open(const wstring& _Str,
		ios_base::openmode _Mode,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a wide-named C stream -- EXTENSION
		return (open(_Str.c_str(), _Mode, _Prot));
		}
 #endif /* _HAS_CPP0X */

	_Myt *open(const wchar_t *_Filename,
		ios_base::open_mode _Mode)
		{	// open a wide-named C stream (old style) -- EXTENSION
		return (open(_Filename, (ios_base::openmode)_Mode));
		}

 #ifdef _NATIVE_WCHAR_T_DEFINED
	_Myt *open(const unsigned short *_Filename,
		ios_base::openmode _Mode,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a wide-named C stream -- EXTENSION
		_Filet *_File;
		if (_Myfile != 0 || (_File = _Fiopen(_Filename, _Mode, _Prot)) == 0)
			return (0);	// open failed

		_Init(_File, _Openfl);
		_Initcvt(&_USE(_Mysb::getloc(), _Cvt));
		return (this);	// open succeeded
		}

	_Myt *open(const unsigned short *_Filename,
		ios_base::open_mode _Mode)
		{	// open a wide-named C stream (old style) -- EXTENSION
		return (open(_Filename, (ios_base::openmode)_Mode));
		}
 #endif /* _NATIVE_WCHAR_T_DEFINED */

	_Myt *close()
		{	// close the C stream
		_Myt *_Ans = this;
		if (_Myfile == 0)
			_Ans = 0;
		else
			{	// put any homing sequence and close file
			if (!_Endwrite())
				_Ans = 0;
			if (fclose(_Myfile) != 0)
				_Ans = 0;
			}
		_Init(0, _Closefl);
		return (_Ans);
		}

	virtual void __CLR_OR_THIS_CALL _Lock()
		{	// lock file instead of stream buffer
		if (_Myfile)
			_CSTD _lock_file(_Myfile);
		}

	virtual void __CLR_OR_THIS_CALL _Unlock()
		{	// unlock file instead of stream buffer
		if (_Myfile)
			_CSTD _unlock_file(_Myfile);
		}

protected:
	virtual int_type __CLR_OR_THIS_CALL overflow(int_type _Meta =
		_Traits::eof())
		{	// put an element to stream
		if (_Traits::eq_int_type(_Traits::eof(), _Meta))
			return (_Traits::not_eof(_Meta));	// EOF, return success code
		else if (_Mysb::pptr() != 0
			&& _Mysb::pptr() < _Mysb::epptr())
			{	// room in buffer, store it
			*_Mysb::_Pninc() = _Traits::to_char_type(_Meta);
			return (_Meta);
			}
		else if (_Myfile == 0)
			return (_Traits::eof());	// no open C stream, fail
		_Reset_back();	// revert from _Mychar buffer
		if (_Pcvt == 0)
			return (_Fputc(_Traits::to_char_type(_Meta), _Myfile)
				? _Meta : _Traits::eof());	// no codecvt facet, put as is
		else
			{	// put using codecvt facet
			const int _STRING_INC = 8;
			const _Elem _Ch = _Traits::to_char_type(_Meta);
			const _Elem *_Src;
			char *_Dest;

			string _Str(_STRING_INC, '\0');
			for (; ; )
				switch (_Pcvt->out(_State,
					&_Ch, &_Ch + 1, _Src,
					&*_Str.begin(), &*_Str.begin() + _Str.size(), _Dest))
				{	// test result of converting one element
				case codecvt_base::partial:
				case codecvt_base::ok:
					{	// converted something, try to put it out
					size_t _Count = _Dest - &*_Str.begin();
					if (0 < _Count && _Count !=
						fwrite(&*_Str.begin(), 1, _Count, _Myfile))
						return (_Traits::eof());	// write failed

					_Wrotesome = true;	// write succeeded
					if (_Src != &_Ch)
						return (_Meta);	// converted whole element

					if (0 < _Count)
						;
					else if (_Str.size() < 4 * _STRING_INC)
						_Str.append(_STRING_INC, '\0');	// try with more space
					else
						return (_Traits::eof());	// conversion failed
					break;
					}

				case codecvt_base::noconv:
					return (_Fputc(_Ch, _Myfile) ? _Meta
						: _Traits::eof());	// no conversion, put as is

				default:
					return (_Traits::eof());	// conversion failed
				}
			}
		}

	virtual int_type __CLR_OR_THIS_CALL pbackfail(int_type _Meta =
		_Traits::eof())
		{	// put an element back to stream
		if (_Mysb::gptr() != 0
			&& _Mysb::eback() < _Mysb::gptr()
			&& (_Traits::eq_int_type(_Traits::eof(), _Meta)
			|| _Traits::eq_int_type(_Traits::to_int_type(_Mysb::gptr()[-1]),
				_Meta)))
			{	// just back up position
			_Mysb::_Gndec();
			return (_Traits::not_eof(_Meta));
			}
		else if (_Myfile == 0 || _Traits::eq_int_type(_Traits::eof(), _Meta))
			return (_Traits::eof());	// no open C stream or EOF, fail
		else if (_Pcvt == 0 && _Ungetc(_Traits::to_char_type(_Meta), _Myfile))
			return (_Meta);	// no facet and unget succeeded, return
		else if (_Mysb::gptr() != &_Mychar)
			{	// putback to _Mychar
			_Mychar = _Traits::to_char_type(_Meta);
			_Set_back();	// switch to _Mychar buffer
			return (_Meta);
			}
		else
			return (_Traits::eof());	// nowhere to put back
		}

	virtual int_type __CLR_OR_THIS_CALL underflow()
		{	// get an element from stream, but don't point past it
		int_type _Meta;
		if (_Mysb::gptr() != 0
			&& _Mysb::gptr() < _Mysb::egptr())
			return (_Traits::to_int_type(*_Mysb::gptr()));	// return buffered
		else if (_Traits::eq_int_type(_Traits::eof(), _Meta = uflow()))
			return (_Meta);	// uflow failed, return EOF
		else
			{	// get a char, don't point past it
			pbackfail(_Meta);
			return (_Meta);
			}
		}

	virtual int_type __CLR_OR_THIS_CALL uflow()
		{	// get an element from stream, point past it
		if (_Mysb::gptr() != 0
			&& _Mysb::gptr() < _Mysb::egptr())
			return (_Traits::to_int_type(
				*_Mysb::_Gninc()));	// return buffered
		else if (_Myfile == 0)
			return (_Traits::eof());	// no open C stream, fail
		_Reset_back();	// revert from _Mychar buffer
		if (_Pcvt == 0)
			{	// no codecvt facet, just get it
			_Elem _Ch = 0;
			return (_Fgetc(_Ch, _Myfile) ? _Traits::to_int_type(_Ch)
				: _Traits::eof());
			}
		else
			{	// build string until codecvt succeeds
			string _Str;

			for (; ; )
				{	// get using codecvt facet
				_Elem _Ch, *_Dest;
				const char *_Src;
				int _Nleft;
				int _Meta = fgetc(_Myfile);

				if (_Meta == EOF)
					return (_Traits::eof());	// partial char?

				_Str.append(1, (char)_Meta);	// append byte and convert
				switch (_Pcvt->in(_State,
					&*_Str.begin(), &*_Str.begin() + _Str.size(), _Src,
					&_Ch, &_Ch + 1, _Dest))
					{	// test result of converting one element
				case codecvt_base::partial:
				case codecvt_base::ok:
					if (_Dest != &_Ch)
						{	// got an element, put back excess and deliver it
						_Nleft = (int)(&*_Str.begin() + _Str.size() - _Src);
						for (; 0 < _Nleft; )
							ungetc(_Src[--_Nleft], _Myfile);
						return (_Traits::to_int_type(_Ch));
						}
					else
						_Str.erase((size_t)0,	// partial, discard used input
							(size_t)(_Src - &*_Str.begin()));
					break;

				case codecvt_base::noconv:
					if (_Str.size() < sizeof (_Elem))
						break;	// no conversion, but need more chars

					_CRT_SECURE_MEMCPY(&_Ch, sizeof (_Elem), &*_Str.begin(),
						sizeof (_Elem));	// copy raw bytes to element

					return (_Traits::to_int_type(_Ch));	// return result

				default:
					return (_Traits::eof());	// conversion failed
					}
				}
			}
		}

	virtual pos_type __CLR_OR_THIS_CALL seekoff(off_type _Off,
		ios_base::seekdir _Way,
		ios_base::openmode =
			(ios_base::openmode)(ios_base::in | ios_base::out))
		{	// change position by _Off
		fpos_t _Fileposition;

		if (_Mysb::gptr() == &_Mychar	// something putback
			&& _Way == ios_base::cur	// a relative seek
			&& _Pcvt == 0)	// not converting
			_Off -= (off_type)sizeof (_Elem);	// back up over _Elem bytes

		if (_Myfile == 0 || !_Endwrite()
			|| (_Off != 0 || _Way != ios_base::cur)
				&& _FSEEK_OFF(_Myfile, _Off, _Way) != 0
			|| fgetpos(_Myfile, &_Fileposition) != 0)
			return (pos_type(_BADOFF));	// report failure

		_Reset_back();	// revert from _Mychar buffer, discarding any putback
		return (_POS_TYPE_FROM_STATE(pos_type, _State,
			_Fileposition));	// return new position
		}

	virtual pos_type __CLR_OR_THIS_CALL seekpos(pos_type _Pos,
		ios_base::openmode =
			(ios_base::openmode)(ios_base::in | ios_base::out))
		{	// change position to _Pos
		fpos_t _Fileposition = _POS_TYPE_TO_FPOS_T(_Pos);
		off_type _Off = (off_type)_Pos - (off_type)_FPOSOFF(_Fileposition);

		if (_Myfile == 0 || !_Endwrite()
			|| fsetpos(_Myfile, &_Fileposition) != 0
			|| _Off != 0 && _FSEEK_OFF(_Myfile, _Off, SEEK_CUR) != 0
			|| fgetpos(_Myfile, &_Fileposition) != 0)
			return (pos_type(_BADOFF));	// report failure

		_State = _POS_TYPE_TO_STATE(_Pos);

		_Reset_back();	// revert from _Mychar buffer, discarding any putback
		return (_POS_TYPE_FROM_STATE(pos_type, _State,
			_Fileposition));	// return new position
		}

	virtual _Mysb *__CLR_OR_THIS_CALL setbuf(_Elem *_Buffer, streamsize _Count)
		{	// offer _Buffer to C stream
		if (_Myfile == 0 || setvbuf(_Myfile, (char *)_Buffer,
			_Buffer == 0 && _Count == 0 ? _IONBF : _IOFBF,
			(size_t)_Count * sizeof (_Elem)) != 0)
			return (0);	// failed
		else
			{	// new buffer, reinitialize pointers
			_Init(_Myfile, _Openfl);
			return (this);
			}
		}

	virtual int __CLR_OR_THIS_CALL sync()
		{	// synchronize C stream with external file
		return (_Myfile == 0
			|| _Traits::eq_int_type(_Traits::eof(), overflow())
			|| 0 <= fflush(_Myfile) ? 0 : -1);
		}

	virtual void __CLR_OR_THIS_CALL imbue(const locale& _Loc)
		{	// set locale to argument (capture nontrivial codecvt facet)
		_Initcvt(&_USE(_Loc, _Cvt));
		}

	void _Init(_Filet *_File, _Initfl _Which)
		{	// initialize to C stream _File after {new, open, close}
		__PURE_APPDOMAIN_GLOBAL static _Myst _Stinit;	// initial state

		_Closef = _Which == _Openfl;
		_Wrotesome = false;

		_Mysb::_Init();	// initialize stream buffer base object

 #ifndef _IORCNT
  #define _IORCNT	_IOCNT	/* read and write counts are the same */
  #define _IOWCNT _IOCNT
 #endif /* _IORCNT */

  #pragma warning(push)
  #pragma warning(disable: 6240)	/* prefast noise VSW 489858 */
		if (_File != 0 && sizeof (_Elem) == 1)
  #pragma warning(pop)

			{	// point inside C stream with [first, first + count) buffer
			_Elem **_Pb = (_Elem **)&_File->_IOBASE;
			_Elem **_Pn = (_Elem **)&_File->_IOPTR;
			int *_Nr = (int *)&_File->_IORCNT;
			int *_Nw = (int *)&_File->_IOWCNT;
			_Mysb::_Init(_Pb, _Pn, _Nr, _Pb, _Pn, _Nw);
			}

		_Myfile = _File;
		_State = _Stinit;
		_Pcvt = 0;	// pointer to codecvt facet
		}

	bool _Endwrite()
		{	// put shift to initial conversion state, as needed
		if (_Pcvt == 0 || !_Wrotesome)
			return (true);
		else
			{	// may have to put
			const int _STRING_INC = 8;
			char *_Dest;
			if (_Traits::eq_int_type(_Traits::eof(), overflow()))
				return (false);

			string _Str(_STRING_INC, '\0');
			for (; ; )
				switch (_Pcvt->unshift(_State,
					&*_Str.begin(), &*_Str.begin() + _Str.size(), _Dest))
				{	// test result of homing conversion
				case codecvt_base::ok:
					_Wrotesome = false;	// homed successfully

				case codecvt_base::partial:	// fall through
					{	// put any generated bytes
					size_t _Count = _Dest - &*_Str.begin();
					if (0 < _Count && _Count !=
						fwrite(&*_Str.begin(), 1, _Count, _Myfile))
						return (false);	// write failed
					if (!_Wrotesome)
						return (true);
					if (_Count == 0)
						_Str.append(_STRING_INC, '\0');	// try with more space
					break;
					}

				case codecvt_base::noconv:
					return (true);	// nothing to do

				default:
					return (false);	// conversion failed
				}
			}
		}

	void _Initcvt(const _Cvt *_Newpcvt)
		{	// initialize codecvt pointer
		if (_Newpcvt->always_noconv())
			_Pcvt = 0;	// nothing to do
		else
			{	// set up for nontrivial codecvt facet
			_Pcvt = _Newpcvt;
			_Mysb::_Init();	// reset any buffering
			}
		}

private:
	void _Reset_back()
		{	// restore buffer after putback
		if (_Mysb::eback() == &_Mychar)
			_Mysb::setg(_Set_eback, _Set_egptr, _Set_egptr);
		}

	void _Set_back()
		{	// set up putback area
		if (_Mysb::eback() != &_Mychar)
			{	// save current get buffer
			_Set_eback = _Mysb::eback();
			_Set_egptr = _Mysb::egptr();
			}
		_Mysb::setg(&_Mychar, &_Mychar, &_Mychar + 1);
		}

	_Elem *_Set_eback;	// saves eback() during one-element putback
	_Elem *_Set_egptr;	// saves egptr()

	const _Cvt *_Pcvt;	// pointer to codecvt facet (may be null)
	_Elem _Mychar;	// putback character, when _Ungetc fails
	bool _Wrotesome;	// true if homing sequence may be needed
	typename _Traits::state_type _State;	// current conversion state
	bool _Closef;	// true if C stream must be closed
	_Filet *_Myfile;	// pointer to C stream
	};

	// basic_filebuf TEMPLATE OPERATORS
template<class _Elem,
	class _Traits> inline
	void swap(basic_filebuf<_Elem, _Traits>& _Left,
		basic_filebuf<_Elem, _Traits>& _Right)
	{	// swap _Left and _Right basic_filebufs
	_Left.swap(_Right);
	}

template<class _Elem,
	class _Traits> inline
	void swap(basic_filebuf<_Elem, _Traits>& _Left,
		basic_filebuf<_Elem, _Traits>&& _Right)
	{	// swap _Left and _Right basic_filebufs
	typedef basic_filebuf<_Elem, _Traits> _Myt;
	_Left.swap(_STD forward<_Myt>(_Right));
	}

template<class _Elem,
	class _Traits> inline
	void swap(basic_filebuf<_Elem, _Traits>&& _Left,
		basic_filebuf<_Elem, _Traits>& _Right)
	{	// swap _Left and _Right basic_filebuf
	typedef basic_filebuf<_Elem, _Traits> _Myt;
	_Right.swap(_STD forward<_Myt>(_Left));
	}

		// TEMPLATE CLASS basic_ifstream
template<class _Elem,
	class _Traits>
	class basic_ifstream
		: public basic_istream<_Elem, _Traits>
	{	// input stream associated with a C stream
public:
	typedef basic_ifstream<_Elem, _Traits> _Myt;
	typedef basic_istream<_Elem, _Traits> _Mybase;
	typedef basic_filebuf<_Elem, _Traits> _Myfb;
	typedef basic_ios<_Elem, _Traits> _Myios;

	basic_ifstream()
		: _Mybase(&_Filebuffer)
		{	// construct unopened
		}

	explicit basic_ifstream(const char *_Filename,
		ios_base::openmode _Mode = ios_base::in,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with named file and specified mode
		if (_Filebuffer.open(_Filename, _Mode | ios_base::in, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}

 #if _HAS_CPP0X
	explicit basic_ifstream(const string& _Str,
		ios_base::openmode _Mode = ios_base::in,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with named file and specified mode
		if (_Filebuffer.open(_Str.c_str(), _Mode | ios_base::in, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}
 #endif /* _HAS_CPP0X */

	explicit basic_ifstream(const wchar_t *_Filename,
		ios_base::openmode _Mode = ios_base::in,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with wide-named file -- EXTENSION
		if (_Filebuffer.open(_Filename, _Mode | ios_base::in, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}

 #if _HAS_CPP0X
	explicit basic_ifstream(const wstring& _Str,
		ios_base::openmode _Mode = ios_base::in,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with wide-named file -- EXTENSION
		if (_Filebuffer.open(_Str.c_str(), _Mode | ios_base::in, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}
 #endif /* _HAS_CPP0X */

 #ifdef _NATIVE_WCHAR_T_DEFINED
	explicit basic_ifstream(const unsigned short *_Filename,
		ios_base::openmode _Mode = ios_base::in,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with wide-named file -- EXTENSION
		if (_Filebuffer.open(_Filename, _Mode | ios_base::in, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}
 #endif /* _NATIVE_WCHAR_T_DEFINED */

	explicit basic_ifstream(_Filet *_File)
		: _Mybase(&_Filebuffer),
			_Filebuffer(_File)
		{	// construct with specified C stream
		}

	basic_ifstream(_Myt&& _Right)
		: _Mybase(&_Filebuffer)
		{	// construct by moving _Right
		_Assign_rv(_STD forward<_Myt>(_Right));
		}

	_Myt& operator=(_Myt&& _Right)
		{	// move from _Right
		_Assign_rv(_STD forward<_Myt>(_Right));
		return (*this);
		}

	void _Assign_rv(_Myt&& _Right)
		{	// assign by moving _Right
		if (this != &_Right)
			{	// different, worth moving
			_Filebuffer.close();
			this->swap(_Right);
			}
		}

	void swap(_Myt& _Right)
		{	// swap with _Right
		if (this != &_Right)
			{	// different, swap base and buffer
			_Mybase::swap(_Right);
			_Filebuffer.swap(_Right._Filebuffer);
			}
		}

	void swap(_Myt&& _Right)
		{	// swap with _Right
		_Assign_rv(_STD forward<_Myt>(_Right));
		}

	void open(const wchar_t *_Filename,
		ios_base::openmode _Mode = ios_base::in,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a wide-named C stream -- EXTENSION
		if (_Filebuffer.open(_Filename, _Mode | ios_base::in, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		else
			_Myios::clear();	// added with C++0X
		}

 #if _HAS_CPP0X
	void open(const wstring& _Str,
		ios_base::openmode _Mode = ios_base::in,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a wide-named C stream -- EXTENSION
		open(_Str.c_str(), _Mode, _Prot);
		}
 #endif /* _HAS_CPP0X */

	void open(const wchar_t *_Filename, ios_base::open_mode _Mode)
		{	// open wide-named file (old style) -- EXTENSION
		open(_Filename, (ios_base::openmode)_Mode);
		}

 #ifdef _NATIVE_WCHAR_T_DEFINED
	void open(const unsigned short *_Filename,
		ios_base::openmode _Mode = ios_base::in,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a wide-named C stream -- EXTENSION
		if (_Filebuffer.open(_Filename, _Mode | ios_base::in, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		else
			_Myios::clear();	// added with C++0X
		}

	void open(const unsigned short *_Filename,
		ios_base::open_mode _Mode)
		{	// open wide-named file (old style) -- EXTENSION
		open(_Filename, (ios_base::openmode)_Mode);
		}
 #endif /* _NATIVE_WCHAR_T_DEFINED */

	virtual __CLR_OR_THIS_CALL ~basic_ifstream()
		{	// destroy the object
		}

	_Myfb *rdbuf() const
		{	// return pointer to file buffer
		return ((_Myfb *)&_Filebuffer);
		}

	bool is_open() const
		{	// test if C stream has been opened
		return (_Filebuffer.is_open());
		}

	void open(const char *_Filename,
		ios_base::openmode _Mode = ios_base::in,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a C stream with specified mode
		if (_Filebuffer.open(_Filename, _Mode | ios_base::in, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		else
			_Myios::clear();	// added with C++0X
		}

 #if _HAS_CPP0X
	void open(const string& _Str,
		ios_base::openmode _Mode = ios_base::in,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a C stream with specified mode
		open(_Str.c_str(), _Mode, _Prot);
		}
 #endif /* _HAS_CPP0X */

	void open(const char *_Filename, ios_base::open_mode _Mode)
		{	// open named file with specified mode (old style)
		open(_Filename, (ios_base::openmode)_Mode);
		}

	void close()
		{	// close the C stream
		if (_Filebuffer.close() == 0)
			_Myios::setstate(ios_base::failbit);
		}

private:
	_Myfb _Filebuffer;	// the file buffer
	};

	// basic_ifstream TEMPLATE OPERATORS
template<class _Elem,
	class _Traits> inline
	void swap(basic_ifstream<_Elem, _Traits>& _Left,
		basic_ifstream<_Elem, _Traits>& _Right)
	{	// swap _Left and _Right basic_ifstreams
	_Left.swap(_Right);
	}

template<class _Elem,
	class _Traits> inline
	void swap(basic_ifstream<_Elem, _Traits>& _Left,
		basic_ifstream<_Elem, _Traits>&& _Right)
	{	// swap _Left and _Right basic_ifstreams
	typedef basic_ifstream<_Elem, _Traits> _Myt;
	_Left.swap(_STD forward<_Myt>(_Right));
	}

template<class _Elem,
	class _Traits> inline
	void swap(basic_ifstream<_Elem, _Traits>&& _Left,
		basic_ifstream<_Elem, _Traits>& _Right)
	{	// swap _Left and _Right basic_ifstreams
	typedef basic_ifstream<_Elem, _Traits> _Myt;
	_Right.swap(_STD forward<_Myt>(_Left));
	}

		// TEMPLATE CLASS basic_ofstream
template<class _Elem,
	class _Traits>
	class basic_ofstream
		: public basic_ostream<_Elem, _Traits>
	{	// output stream associated with a C stream
public:
	typedef basic_ofstream<_Elem, _Traits> _Myt;
	typedef basic_ostream<_Elem, _Traits> _Mybase;
	typedef basic_filebuf<_Elem, _Traits> _Myfb;
	typedef basic_ios<_Elem, _Traits> _Myios;

	basic_ofstream()
		: _Mybase(&_Filebuffer)
		{	// construct unopened
		}

	explicit basic_ofstream(const char *_Filename,
		ios_base::openmode _Mode = ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with named file and specified mode
		if (_Filebuffer.open(_Filename, _Mode | ios_base::out, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}

 #if _HAS_CPP0X
	explicit basic_ofstream(const string& _Str,
		ios_base::openmode _Mode = ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with named file and specified mode
		if (_Filebuffer.open(_Str.c_str(), _Mode | ios_base::out, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}
 #endif /* _HAS_CPP0X */

	explicit basic_ofstream(const wchar_t *_Filename,
		ios_base::openmode _Mode = ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with wide-named file -- EXTENSION
		if (_Filebuffer.open(_Filename, _Mode | ios_base::out, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}

 #if _HAS_CPP0X
	explicit basic_ofstream(const wstring& _Str,
		ios_base::openmode _Mode = ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with wide-named file -- EXTENSION
		if (_Filebuffer.open(_Str.c_str(), _Mode | ios_base::out, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}
 #endif /* _HAS_CPP0X */

 #ifdef _NATIVE_WCHAR_T_DEFINED
	explicit basic_ofstream(const unsigned short *_Filename,
		ios_base::openmode _Mode = ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with wide-named file -- EXTENSION
		if (_Filebuffer.open(_Filename, _Mode | ios_base::out, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}
 #endif /* _NATIVE_WCHAR_T_DEFINED */

	explicit basic_ofstream(_Filet *_File)
		: _Mybase(&_Filebuffer),
			_Filebuffer(_File)
		{	// construct with specified C stream
		}

	basic_ofstream(_Myt&& _Right)
		: _Mybase(&_Filebuffer)
		{	// construct by moving _Right
		_Assign_rv(_STD forward<_Myt>(_Right));
		}

	_Myt& operator=(_Myt&& _Right)
		{	// move from _Right
		_Assign_rv(_STD forward<_Myt>(_Right));
		return (*this);
		}

	void _Assign_rv(_Myt&& _Right)
		{	// assign by moving _Right
		if (this != &_Right)
			{	// different, worth moving
			_Filebuffer.close();
			this->swap(_Right);
			}
		}

	void swap(_Myt& _Right)
		{	// swap with _Right
		if (this != &_Right)
			{	// different, swap base and buffer
			_Mybase::swap(_Right);
			_Filebuffer.swap(_Right._Filebuffer);
			}
		}

	void swap(_Myt&& _Right)
		{	// swap with _Right
		_Assign_rv(_STD forward<_Myt>(_Right));
		}

	void open(const wchar_t *_Filename,
		ios_base::openmode _Mode = ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a wide-named C stream -- EXTENSION
		if (_Filebuffer.open(_Filename, _Mode | ios_base::out, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		else
			_Myios::clear();	// added with C++0X
		}

 #if _HAS_CPP0X
	void open(const wstring& _Str,
		ios_base::openmode _Mode = ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a wide-named C stream -- EXTENSION
		open(_Str.c_str(), _Mode, _Prot);
		}
 #endif /* _HAS_CPP0X */

	void open(const wchar_t *_Filename, ios_base::open_mode _Mode)
		{	// open a wide-named C stream (old style) -- EXTENSION
		open(_Filename, (ios_base::openmode)_Mode);
		}

 #ifdef _NATIVE_WCHAR_T_DEFINED
	void open(const unsigned short *_Filename,
		ios_base::openmode _Mode = ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a wide-named C stream -- EXTENSION
		if (_Filebuffer.open(_Filename, _Mode | ios_base::out, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		else
			_Myios::clear();	// added with C++0X
		}

	void open(const unsigned short *_Filename,
		ios_base::open_mode _Mode)
		{	// open a wide-named C stream (old style) -- EXTENSION
		open(_Filename, (ios_base::openmode)_Mode);
		}
 #endif /* _NATIVE_WCHAR_T_DEFINED */

	virtual __CLR_OR_THIS_CALL ~basic_ofstream()
		{	// destroy the object
		}

	_Myfb *rdbuf() const
		{	// return pointer to file buffer
		return ((_Myfb *)&_Filebuffer);
		}

	bool is_open() const
		{	// test if C stream has been opened
		return (_Filebuffer.is_open());
		}

	void open(const char *_Filename,
		ios_base::openmode _Mode = ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a C stream with specified mode
		if (_Filebuffer.open(_Filename, _Mode | ios_base::out, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		else
			_Myios::clear();	// added with C++0X
		}

 #if _HAS_CPP0X
	void open(const string& _Str,
		ios_base::openmode _Mode = ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a C stream with specified mode
		open(_Str.c_str(), _Mode, _Prot);
		}
 #endif /* _HAS_CPP0X */

	void open(const char *_Filename, ios_base::open_mode _Mode)
		{	// open a C stream with specified mode (old style)
		open(_Filename, (ios_base::openmode)_Mode);
		}

	void close()
		{	// close the C stream
		if (_Filebuffer.close() == 0)
			_Myios::setstate(ios_base::failbit);
		}

private:
	_Myfb _Filebuffer;	// the file buffer
	};

	// basic_ofstream TEMPLATE OPERATORS
template<class _Elem,
	class _Traits> inline
	void swap(basic_ofstream<_Elem, _Traits>& _Left,
		basic_ofstream<_Elem, _Traits>& _Right)
	{	// swap _Left and _Right basic_ofstreams
	_Left.swap(_Right);
	}

template<class _Elem,
	class _Traits> inline
	void swap(basic_ofstream<_Elem, _Traits>& _Left,
		basic_ofstream<_Elem, _Traits>&& _Right)
	{	// swap _Left and _Right basic_ofstreams
	typedef basic_ofstream<_Elem, _Traits> _Myt;
	_Left.swap(_STD forward<_Myt>(_Right));
	}

template<class _Elem,
	class _Traits> inline
	void swap(basic_ofstream<_Elem, _Traits>&& _Left,
		basic_ofstream<_Elem, _Traits>& _Right)
	{	// swap _Left and _Right basic_ofstreams
	typedef basic_ofstream<_Elem, _Traits> _Myt;
	_Right.swap(_STD forward<_Myt>(_Left));
	}

		// TEMPLATE CLASS basic_fstream
template<class _Elem,
	class _Traits>
	class basic_fstream
		: public basic_iostream<_Elem, _Traits>
	{	// input/output stream associated with a C stream
public:
	typedef basic_fstream<_Elem, _Traits> _Myt;
	typedef basic_iostream<_Elem, _Traits> _Mybase;
	typedef basic_filebuf<_Elem, _Traits> _Myfb;
	typedef basic_ios<_Elem, _Traits> _Myios;
	typedef _Elem char_type;
	typedef _Traits traits_type;
	typedef typename _Traits::int_type int_type;
	typedef typename _Traits::pos_type pos_type;
	typedef typename _Traits::off_type off_type;

	basic_fstream()
		: _Mybase(&_Filebuffer)
		{	// construct unopened
		}

	explicit basic_fstream(const char *_Filename,
		ios_base::openmode _Mode = ios_base::in | ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with named file and specified mode
		if (_Filebuffer.open(_Filename, _Mode, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}

 #if _HAS_CPP0X
	explicit basic_fstream(const string& _Str,
		ios_base::openmode _Mode = ios_base::in | ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with named file and specified mode
		if (_Filebuffer.open(_Str.c_str(), _Mode, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}
 #endif /* _HAS_CPP0X */

	explicit basic_fstream(const wchar_t *_Filename,
		ios_base::openmode _Mode = ios_base::in | ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with wide-named file -- EXTENSION
		if (_Filebuffer.open(_Filename, _Mode, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}

 #if _HAS_CPP0X
	explicit basic_fstream(const wstring& _Str,
		ios_base::openmode _Mode = ios_base::in | ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with wide-named file -- EXTENSION
		if (_Filebuffer.open(_Str.c_str(), _Mode, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}
 #endif /* _HAS_CPP0X */

 #ifdef _NATIVE_WCHAR_T_DEFINED
	explicit basic_fstream(const unsigned short *_Filename,
		ios_base::openmode _Mode = ios_base::in | ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		: _Mybase(&_Filebuffer)
		{	// construct with wide-named file -- EXTENSION
		if (_Filebuffer.open(_Filename, _Mode, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		}
 #endif /* _NATIVE_WCHAR_T_DEFINED */

	explicit basic_fstream(_Filet *_File)
		: _Mybase(&_Filebuffer),
			_Filebuffer(_File)
		{	// construct with specified C stream
		}

	basic_fstream(_Myt&& _Right)
		: _Mybase(&_Filebuffer)
		{	// construct by moving _Right
		_Assign_rv(_STD forward<_Myt>(_Right));
		}

	_Myt& operator=(_Myt&& _Right)
		{	// move from _Right
		_Assign_rv(_STD forward<_Myt>(_Right));
		return (*this);
		}

	void _Assign_rv(_Myt&& _Right)
		{	// assign by moving _Right
		if (this != &_Right)
			{	// different, worth moving
			_Filebuffer.close();
			this->swap(_Right);
			}
		}

	void swap(_Myt& _Right)
		{	// swap with _Right
		if (this != &_Right)
			{	// different, swap base and buffer
			_Mybase::swap(_Right);
			_Filebuffer.swap(_Right._Filebuffer);
			}
		}

	void swap(_Myt&& _Right)
		{	// swap with _Right
		_Assign_rv(_STD forward<_Myt>(_Right));
		}

	void open(const wchar_t *_Filename,
		ios_base::openmode _Mode = ios_base::in | ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a wide-named C stream -- EXTENSION
		if (_Filebuffer.open(_Filename, _Mode, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		else
			_Myios::clear();	// added with C++0X
		}

 #if _HAS_CPP0X
	void open(const wstring& _Str,
		ios_base::openmode _Mode = ios_base::in | ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a wide-named C stream -- EXTENSION
		open(_Str.c_str(), _Mode, _Prot);
		}
 #endif /* _HAS_CPP0X */

	void open(const wchar_t *_Filename, ios_base::open_mode _Mode)
		{	// open a wide-named C stream (old style) -- EXTENSION
		open(_Filename, (ios_base::openmode)_Mode);
		}

 #ifdef _NATIVE_WCHAR_T_DEFINED
	void open(const unsigned short *_Filename,
		ios_base::openmode _Mode = ios_base::in | ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a wide-named C stream -- EXTENSION
		if (_Filebuffer.open(_Filename, _Mode, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		else
			_Myios::clear();	// added with C++0X
		}

	void open(const unsigned short *_Filename,
		ios_base::open_mode _Mode)
		{	// open a wide-named C stream (old style) -- EXTENSION
		open(_Filename, (ios_base::openmode)_Mode);
		}
 #endif /* _NATIVE_WCHAR_T_DEFINED */

	virtual __CLR_OR_THIS_CALL ~basic_fstream()
		{	// destroy the object
		}

	_Myfb *rdbuf() const
		{	// return pointer to file buffer
		return ((_Myfb *)&_Filebuffer);
		}

	bool is_open() const
		{	// test if C stream has been opened
		return (_Filebuffer.is_open());
		}

	void open(const char *_Filename,
		ios_base::openmode _Mode = ios_base::in | ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a C stream with specified mode
		if (_Filebuffer.open(_Filename, _Mode, _Prot) == 0)
			_Myios::setstate(ios_base::failbit);
		else
			_Myios::clear();	// added with C++0X
		}

 #if _HAS_CPP0X
	void open(const string& _Str,
		ios_base::openmode _Mode = ios_base::in | ios_base::out,
		int _Prot = (int)ios_base::_Openprot)
		{	// open a C stream with specified mode
		open(_Str.c_str(), _Mode, _Prot);
		}
 #endif /* _HAS_CPP0X */

	void open(const char *_Filename, ios_base::open_mode _Mode)
		{	// open a C stream with specified mode (old style)
		open(_Filename, (ios_base::openmode)_Mode);
		}

	void close()
		{	// close the C stream
		if (_Filebuffer.close() == 0)
			_Myios::setstate(ios_base::failbit);
		}

private:
	_Myfb _Filebuffer;	// the file buffer
	};

	// basic_fstream TEMPLATE OPERATORS
template<class _Elem,
	class _Traits> inline
	void swap(basic_fstream<_Elem, _Traits>& _Left,
		basic_fstream<_Elem, _Traits>& _Right)
	{	// swap _Left and _Right basic_fstreams
	_Left.swap(_Right);
	}

template<class _Elem,
	class _Traits> inline
	void swap(basic_fstream<_Elem, _Traits>& _Left,
		basic_fstream<_Elem, _Traits>&& _Right)
	{	// swap _Left and _Right basic_fstreams
	typedef basic_fstream<_Elem, _Traits> _Myt;
	_Left.swap(_STD forward<_Myt>(_Right));
	}

template<class _Elem,
	class _Traits> inline
	void swap(basic_fstream<_Elem, _Traits>&& _Left,
		basic_fstream<_Elem, _Traits>& _Right)
	{	// swap _Left and _Right basic_fstreams
	typedef basic_fstream<_Elem, _Traits> _Myt;
	_Right.swap(_STD forward<_Myt>(_Left));
	}
_STD_END

 #pragma warning(pop)
 #pragma pack(pop)

#endif /* RC_INVOKED */
#endif /* _FSTREAM_ */

/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.20:0009 */
