#include "pch.h"
#include <fstream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <span>
#include "sdk.h"
#include <format>
#include <chrono>
#include <array>
#include <map>
#include <set>
#include <sstream>
#include <assert.h>

class CMemAlloc : VClass {
public:
	static CMemAlloc* GetMemAlloc( ) {
		static auto p_mem_alloc = *reinterpret_cast<CMemAlloc**>( GetProcAddress( GetModuleHandleW( L"tier0.dll" ), "g_pMemAlloc" ) );
		return p_mem_alloc;
	}

	void* Alloc( std::size_t size ) {
		return GetVF<void* ( __thiscall* )( CMemAlloc*, std::size_t )>( 1 )( this, size );
	}

	void* ReAlloc( const void* p, std::size_t size ) {
		return GetVF<void* ( __thiscall* )( CMemAlloc*, const void*, std::size_t )>( 3 )( this, p, size );
	}

	void Free( const void* p ) {
		return GetVF<void( __thiscall* )( CMemAlloc*, const void* )>( 5 )( this, p );
	}

	std::size_t GetSize( const void* p ) {
		return GetVF<std::size_t( __thiscall* )( CMemAlloc*, const void* )>( 21 )( this, p );
	}
};

template <class T>
class CUtlVector {
public:
	CUtlVector( ) {
		m_Size = 0;
		m_pElements = nullptr;
	}

	T& Element( int i );
	const T& Element( int i ) const;

	T& operator[]( int i ) {
		return m_pElements[i];
	}

	[[nodiscard]] int Count( ) const {
		return m_Size;
	}

	int m_Size;
	T* m_pElements;

	// Adds an element, uses default constructor
	int AddToHead( );
	int AddToTail( );
	int InsertBefore( int elem );
	int InsertAfter( int elem );
	// Adds an element, uses copy constructor
	int AddToHead( const T& src );
	int AddToTail( const T& src );
	int InsertBefore( int elem, const T& src );
	int InsertAfter( int elem, const T& src );

	// Grows the vector
	void GrowVector( int num = 1 );

	// Shifts elements....
	void ShiftElementsRight( int elem, int num = 1 );
	void ShiftElementsLeft( int elem, int num = 1 );

	// Element removal
	void FastRemove( int elem ); // doesn't preserve order
	void Remove( int elem ); // preserves order, shifts elements
	bool FindAndRemove( const T& src ); // removes first occurrence of src, preserves order, shifts elements
	bool FindAndFastRemove( const T& src ); // removes first occurrence of src, doesn't preserve order

	// Finds an element (element needs operator== defined)
	int GetOffset( const T& src ) const;
};

template <typename T>
inline T& CUtlVector<T>::Element( int i ) {
	assert( i < m_Size );
	return m_pElements[i];
}

template <typename T>
inline const T& CUtlVector<T>::Element( int i ) const {
	assert( i < m_Size );
	return m_pElements[i];
}

template <typename T>
void CUtlVector<T>::GrowVector( int num ) {
	m_Size += num;
	if ( m_pElements )
		m_pElements = (T*)CMemAlloc::GetMemAlloc( )->ReAlloc( m_pElements, m_Size * sizeof( T ) );
	else
		m_pElements = (T*)CMemAlloc::GetMemAlloc( )->Alloc( m_Size * sizeof( T ) );
}

//-----------------------------------------------------------------------------
// Adds an element, uses default constructor
//-----------------------------------------------------------------------------
template <typename T>
inline int CUtlVector<T>::AddToHead( ) {
	return InsertBefore( 0 );
}

template <typename T>
inline int CUtlVector<T>::AddToTail( ) {
	return InsertBefore( m_Size );
}

template <typename T>
inline int CUtlVector<T>::InsertAfter( int elem ) {
	return InsertBefore( elem + 1 );
}

template <typename T>
int CUtlVector<T>::InsertBefore( int elem ) {
	// Can insert at the end
	GrowVector( );
	ShiftElementsRight( elem );
	Construct( &Element( elem ) );
	return elem;
}

//-----------------------------------------------------------------------------
// Adds an element, uses copy constructor
//-----------------------------------------------------------------------------
template <typename T>
inline int CUtlVector<T>::AddToHead( const T& src ) {
	return InsertBefore( 0, src );
}

template <typename T>
inline int CUtlVector<T>::AddToTail( const T& src ) {
	return InsertBefore( m_Size, src );
}

template <typename T>
inline int CUtlVector<T>::InsertAfter( int elem, const T& src ) {
	return InsertBefore( elem + 1, src );
}

template <typename T>
int CUtlVector<T>::InsertBefore( int elem, const T& src ) {
	GrowVector( );
	ShiftElementsRight( elem );
	CopyConstruct( &Element( elem ), src );
	return elem;
}

//-----------------------------------------------------------------------------
// Shifts elements
//-----------------------------------------------------------------------------
template <typename T>
void CUtlVector<T>::ShiftElementsRight( int elem, int num ) {
	int numToMove = m_Size - elem - num;
	if ( ( numToMove > 0 ) && ( num > 0 ) )
		memmove( &Element( elem + num ), &Element( elem ), numToMove * sizeof( T ) );
}

template <typename T>
void CUtlVector<T>::ShiftElementsLeft( int elem, int num ) {
	int numToMove = m_Size - elem - num;
	if ( ( numToMove > 0 ) && ( num > 0 ) ) {
		memmove( &Element( elem ), &Element( elem + num ), numToMove * sizeof( T ) );

#ifdef _DEBUG
		memset( &Element( m_Size - num ), 0xDD, num * sizeof( T ) );
#endif
	}
}

//-----------------------------------------------------------------------------
// Element removal
//-----------------------------------------------------------------------------
template <typename T>
void CUtlVector<T>::FastRemove( int elem ) {
	Destruct( &Element( elem ) );
	if ( m_Size > 0 ) {
		if ( elem != m_Size - 1 )
			memcpy( &Element( elem ), &Element( m_Size - 1 ), sizeof( T ) );
		--m_Size;
	}
}

template <typename T>
void CUtlVector<T>::Remove( int elem ) {
	Destruct( &Element( elem ) );
	ShiftElementsLeft( elem );
	--m_Size;
}

template <typename T>
bool CUtlVector<T>::FindAndRemove( const T& src ) {
	int elem = GetOffset( src );
	if ( elem != -1 ) {
		Remove( elem );
		return true;
	}
	return false;
}

template <typename T>
bool CUtlVector<T>::FindAndFastRemove( const T& src ) {
	int elem = GetOffset( src );
	if ( elem != -1 ) {
		FastRemove( elem );
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Finds an element (element needs operator== defined)
//-----------------------------------------------------------------------------
template <typename T>
int CUtlVector<T>::GetOffset( const T& src ) const {
	for ( auto i = 0; i < Count( ); ++i ) {
		if ( Element( i ) == src )
			return i;
	}
	return -1;
}
using UtlTsHashHandleT = std::uint64_t;

class CUtlMemoryPool {
public:
	// returns number of allocated blocks
	int BlockSize( ) const {
		return m_blocks_per_blob_;
	}
	int Count( ) const {
		return m_block_allocated_size_;
	}
	int PeakCount( ) const {
		return m_peak_alloc_;
	}
private:
	std::int32_t m_block_size_ = 0; // 0x0558
	std::int32_t m_blocks_per_blob_ = 0; // 0x055C
	std::int32_t m_grow_mode_ = 0; // 0x0560
	std::int32_t m_blocks_allocated_ = 0; // 0x0564
	std::int32_t m_block_allocated_size_ = 0; // 0x0568
	std::int32_t m_peak_alloc_ = 0; // 0x056C
};

template <class T, class Keytype = std::uint64_t>
class CUtlTSHash {
public:
	// Invalid handle.
	static UtlTsHashHandleT InvalidHandle( void ) {
		return static_cast<UtlTsHashHandleT>( 0 );
	}

	// Returns the number of elements in the hash table
	[[nodiscard]] int BlockSize( ) const {
		return m_entry_memory_.BlockSize( );
	}
	[[nodiscard]] int Count( ) const {
		return m_entry_memory_.Count( );
	}

	// Returns elements in the table
	std::vector<T> GetElements( void );
public:
	// Templatized for memory tracking purposes
	template <typename DataT>
	struct HashFixedDataInternalT {
		Keytype m_ui_key;
		HashFixedDataInternalT<DataT>* m_next;
		DataT m_data;
	};

	using HashFixedDataT = HashFixedDataInternalT<T>;

	// Templatized for memory tracking purposes
	template <typename DataT>
	struct HashFixedStructDataInternalT {
		DataT m_data;
		Keytype m_ui_key;
		char pad_0x0020[0x8];
	};

	using HashFixedStructDataT = HashFixedStructDataInternalT<T>;

	struct HashStructDataT {
		char pad_0x0000[0x10]; // 0x0000
		std::array<HashFixedStructDataT, 256> m_list;
	};

	struct HashAllocatedDataT {
	public:
		auto GetList( ) {
			return m_list_;
		}
	private:
		char pad_0x0000[0x18]; // 0x0000
		std::array<HashFixedDataT, 128> m_list_;
	};

	// Templatized for memory tracking purposes
	template <typename DataT>
	struct HashBucketDataInternalT {
		DataT m_data;
		HashFixedDataInternalT<DataT>* m_next;
		Keytype m_ui_key;
	};

	using HashBucketDataT = HashBucketDataInternalT<T>;

	struct HashUnallocatedDataT {
		HashUnallocatedDataT* m_next_ = nullptr; // 0x0000
		Keytype m_6114; // 0x0008
		Keytype m_ui_key; // 0x0010
		Keytype m_i_unk_1; // 0x0018
		std::array<HashBucketDataT, 256> m_current_block_list; // 0x0020
	};

	struct HashBucketT {
		HashStructDataT* m_struct_data = nullptr;
		void* m_mutex_list = nullptr;
		HashAllocatedDataT* m_allocated_data = nullptr;
		HashUnallocatedDataT* m_unallocated_data = nullptr;
	};

	CUtlMemoryPool m_entry_memory_;
	HashBucketT m_buckets_;
	bool m_needs_commit_ = false;
};

template <class T, class Keytype>
std::vector<T> CUtlTSHash<T, Keytype>::GetElements( void ) {
	std::vector<T> list;

	const int n_count = Count( );
	auto n_index = 0;
	auto& unallocated_data = m_buckets_.m_unallocated_data;
	for ( auto element = unallocated_data; element; element = element->m_next_ ) {
		for ( auto i = 0; i < BlockSize( ) && i != n_count; i++ ) {
			list.emplace_back( element->m_current_block_list.at( i ).m_data );
			n_index++;

			if ( n_index >= n_count )
				break;
		}
	}
	return list;
}

struct CSchemaClassBinding {
	CSchemaClassBinding* parent;
	const char* m_binary_name; // ex: C_World
	const char* m_module_name; // ex: libclient.so
	const char* m_class_name; // ex: client
	void* m_class_info_old_synthesized;
	void* m_class_info;
	void* m_this_module_binding_pointer;
	void* m_schema_type;
};

struct ClassDescription;

class CSchemaSystemTypeScope : VClass {
public:
	ClassDescription* FindDeclaredClass( const char* class_name ) {
		return CallVFunc<2, ClassDescription*>( class_name );
	}

	std::string_view GetScopeName( ) {
		return { m_name_.data( ) };
	}

	CUtlTSHash<CSchemaClassBinding*> GetClasses( ) {
		return Member< CUtlTSHash<CSchemaClassBinding*> >( 0x558 );
	}

	std::array<char, 256> m_name_ = {};
};


struct SchemaParentInfo {
	uintptr_t idk;
	ClassDescription* parent;
};

struct SchemaTypeDescription {
	uintptr_t idk;
	const char* name;
	uintptr_t idk2;
};


struct SchemaClassFieldData_t {
	const char* m_name; // 0x0000
	SchemaTypeDescription* m_type; // 0x0008
	std::int32_t m_single_inheritance_offset; // 0x0010
	std::int32_t m_metadata_size; // 0x0014
	void* m_metadata; // 0x0018
};

struct ClassDescription {
	ClassDescription* self;        //0
	const char* className;         //8
	const char* modulename;        //10
	uint32_t classSize;               //18
	short membersSize;		   //1c
	char pad[6];				   //20
	SchemaClassFieldData_t* membersDescription; //28
	uintptr_t idk2;                //30
	SchemaParentInfo* parentInfo;  //38
};

struct less_than_key
{
	inline bool operator() ( const SchemaClassFieldData_t& struct1, const SchemaClassFieldData_t& struct2 ) const
	{
		return ( struct1.m_single_inheritance_offset < struct2.m_single_inheritance_offset );
	}
};

std::unordered_map<std::string, std::set<SchemaClassFieldData_t, less_than_key>> Netvars;

typedef void* ( *oCreateInterface )( const char*, int );
oCreateInterface pCreateInterface;
uintptr_t CreateInterface( const char* szModule, const char* szInterface ) {
	pCreateInterface = (oCreateInterface)GetProcAddress( GetModuleHandleA( szModule ), "CreateInterface" );
	return (uintptr_t)pCreateInterface( szInterface, 0 );
}

class CSchemaSystem : public VClass {
public:
	CSchemaSystemTypeScope* GlobalTypeScope( void ) {
		return CallVFunc<11, CSchemaSystemTypeScope*>( );
	}

	CSchemaSystemTypeScope* FindTypeScopeForModule( const char* m_module_name ) {
		return CallVFunc<13, CSchemaSystemTypeScope*>( m_module_name );
	}

	CUtlVector<CSchemaSystemTypeScope*> type_scopes( ) {
		return Member<CUtlVector<CSchemaSystemTypeScope*>>( 0x190 );
	}
};

CSchemaSystem* SchemaSystem = 0;

inline void DumpClassMembers( ClassDescription* classDesc ) {
	std::cout << "Dumping " << classDesc->className << "...\n";

	std::string className = classDesc->className;
	for ( uintptr_t i = 0; i < classDesc->membersSize; i++ ) {
		SchemaClassFieldData_t desc = classDesc->membersDescription[i];
		Netvars[className].insert( desc );
		//		std::cout << std::format("{}: {} ({})\n", info.schematypeptr->name, info.name, info.offset);
	}

	if ( classDesc->parentInfo ) {
		classDesc = classDesc->parentInfo->parent;
		DumpClassMembers( classDesc );
	}

}

template<typename... Args>
inline void SchemaDumpToMap( const char* _module, Args&&... args ) {
	const char* classes[sizeof...( args )] = { std::forward<Args>( args )... };
	auto typeScope = SchemaSystem->CallVFunc<13, CSchemaSystemTypeScope*>( _module );

	if ( !typeScope )
		return;

	for ( auto& _class : classes ) {
		//std::cout << "Scope " << std::hex << Scope << std::dec << std::endl;
		auto classDesc = typeScope->FindDeclaredClass( _class );
		if ( !classDesc ) {
			std::cout << "No such class: " << _class << "\n";
			return;
		}
		DumpClassMembers( classDesc );
	}
}

std::string getTimeStr( ) {
	std::time_t now = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now( ) );

	std::string s( 30, '\0' );
	std::strftime( &s[0], s.size( ), "%d-%m-%Y %H:%M:%S", std::localtime( &now ) );
	return s;
}

uintptr_t WINAPI HackThread( HMODULE hModule ) {
	const bool console = true;

	FILE* f;
	if ( console ) {
		AllocConsole( );
		freopen_s( &f, "CONOUT$", "w", stdout );
	}


	SchemaSystem = (CSchemaSystem*)CreateInterface( "schemasystem.dll", "SchemaSystem_001" );
	std::cout << "Dumping...\n";

	const auto type_scopes = SchemaSystem->type_scopes( );

	clock_t tic = clock( );

	std::ofstream fout( "C:\\Users\\user\\Documents\\asd\\fields.hpp" );
	fout << std::hex;
	fout << "#pragma once\n#include <cstdint>\nnamespace fields {\n";

	for ( auto i = 0; i < type_scopes.Count( ); ++i ) {
		const auto current_scope = type_scopes.m_pElements[i];
		auto current_classes = current_scope->GetClasses( );

		for ( const auto class__ : current_classes.GetElements( ) ) {
			const auto class_info = current_scope->FindDeclaredClass( class__->m_binary_name );
			fout << std::format( "\tnamespace {}", class__->m_binary_name ) << " {\n";

			if ( !class_info->membersSize ) fout << "\t\t// empty class" << '\n';
			for ( auto ki = 0; ki < class_info->membersSize; ki++ ) {
				if ( !&class_info->membersDescription[ki] ) continue;

				fout << std::format( "\t\tconstexpr uint32_t {} = ", ( &class_info->membersDescription[ki] )->m_name ) << "0x" << ( &class_info->membersDescription[ki] )->m_single_inheritance_offset << ";";
				fout << " // " << ( &class_info->membersDescription[ki] )->m_type->name << '\n';
			}
			fout << "\t}\n";
		}
	}
	fout << "}";
	clock_t toc = clock( );

	std::cout << "Time elapsed " << round( ( (double)( toc - tic ) / CLOCKS_PER_SEC ) * 10 ) / 10 << "s" << std::endl;

	// You can add some other classes there if you need to
	/*SchemaDumpToMap( "client.dll",
		"CEntityIdentity",
		"C_DOTA_Item",
		"C_DOTA_BaseNPC_Hero",
		"C_DOTAPlayerController",
		"C_DOTA_UnitInventory",
		"C_DOTABaseAbility",
		"C_DOTA_PlayerResource",
		"PlayerResourcePlayerTeamData_t",
		"PlayerResourcePlayerData_t",
		"C_DOTAGamerules",
		"CGameSceneNode",
		"C_DOTA_Item_Rune",
		"GameTime_t",
		"C_DOTA_Item_EmptyBottle");

	SchemaDumpToMap("server.dll", "CDOTA_Buff");

	std::ofstream fout("H:\\Reversing\\SchemaDump\\header.h");
	fout << std::hex;
	fout << "#pragma once\n#include <cstdint>\nnamespace Netvars {\n";
	for (auto& [className, classMap] : Netvars) {
		fout << std::format("\tnamespace {}", className) << " {\n";
		for (auto& desc : classMap) {
			fout << std::format("\t\tconstexpr uint32_t {} = ", desc.name) << "0x" << desc.offset << ";";
			fout << " // " << desc.typeDesc->name << '\n';
		}
		fout << "\t}\n";
	}
	fout << "}";
	std::cout << "Done!\n";*/

	if ( console ) {
		system( "pause" );
		if ( f ) fclose( f );
		FreeConsole( );
	}
	FreeLibraryAndExitThread( hModule, 0 );
	return 0;
}
BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
)
{
	switch ( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH: {

		HANDLE handle = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0 );
		if ( handle ) CloseHandle( handle );
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

