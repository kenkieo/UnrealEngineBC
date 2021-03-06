// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

/*=============================================================================
	BuildPatchUtil.h: Declares miscellaneous utility functions.
=============================================================================*/

#pragma once

/**
 * Some utility functions
 */
struct FBuildPatchUtils
{
	/**
	 * Gets the filename for a chunk generated from it's GUID and Hash, which is the new format.
	 * @param ManifestVersion	The manifest version that references this chunk
	 * @param RootDirectory		The root directory
	 * @param ChunkGuid			The chunk Guid
	 * @param ChunkHash			The chunk rolling hash value
	 * @return	the chunk path
	 */
	static const FString GetChunkNewFilename(const EBuildPatchAppManifestVersion::Type ManifestVersion, const FString& RootDirectory, const FGuid& ChunkGUID, const uint64& ChunkHash);

	/**
	 * Gets the filename for a file chunk generated from it's GUID and Hash, which is the new format.
	 * @param ManifestVersion	The manifest version that references this file
	 * @param RootDirectory		The root directory
	 * @param FileGUID			The file chunk Guid
	 * @param FileHash			The file hash value
	 * @return	The file chunk path
	 */
	static const FString GetFileNewFilename(const EBuildPatchAppManifestVersion::Type ManifestVersion, const FString& RootDirectory, const FGuid& FileGUID, const FSHAHashData& FileHash);
	static const FString GetFileNewFilename(const EBuildPatchAppManifestVersion::Type ManifestVersion, const FString& RootDirectory, const FGuid& FileGUID, const uint64& FilePartHash);

	/**
	 * Gets the chunk GUID and Hash, from the filename, which is the new format.
	 * @param ChunkNewFilename	IN	The chunk filename that would have been generated from GetChunkNewFilename
	 * @param ChunkGuid			OUT	The chunk Guid
	 * @param ChunkHash			OUT	The chunk rolling hash value
	 */
	static void GetChunkDetailFromNewFilename(const FString& ChunkNewFilename, FGuid& ChunkGUID, uint64& ChunkHash);

	/**
	 * Gets the file chunk GUID and file hash, from the filename, which is the new format.
	 * @param FileNewFilename	IN	The file chunk filename that would have been generated from GetChunkNewFilename
	 * @param FileGUID			OUT	The file chunk Guid
	 * @param FileHash			OUT	The file chunk rolling hash value
	 */
	static void GetFileDetailFromNewFilename(const FString& FileNewFilename, FGuid& FileGUID, FSHAHashData& FileHash);

	/**
	 * Gets the filename for a chunk generated from it's GUID
	 * @param RootDirectory		The root directory
	 * @param ChunkGuid			The chunk Guid
	 * @return	the chunk path
	 */
	static const FString GetChunkOldFilename(const FString& RootDirectory, const FGuid& ChunkGUID);

	/**
	 * Gets the filename for a file data part generated from it's GUID
	 * @param RootDirectory		The root directory
	 * @param FileGUID			The file part Guid
	 * @return	The file data path
	 */
	static const FString GetFileOldFilename(const FString& RootDirectory, const FGuid& FileGUID);

	/**
	 * Gets the filename for a specific data part type from it's GUID
	 * @param DataType			The type of data
	 * @param RootDirectory		The root directory
	 * @param DataGUID			The data Guid
	 * @return	the data part path
	 */
	static const FString GetDataTypeOldFilename(const FBuildPatchData::Type DataType, const FString& RootDirectory, const FGuid& Guid);

	/**
	 * Gets the filename for any data part. Wraps the choice between all of the above
	 * @param Manifest			The manifest referencing this data
	 * @param RootDirectory		The root directory
	 * @param DataGUID			The data Guid
	 * @return	the data part path
	 */
	static const FString GetDataFilename(const FBuildPatchAppManifestRef& Manifest, const FString& RootDirectory, const FGuid& DataGUID);

	/**
	 * Gets the GUID for a data file according to it's filename (new or old)
	 * @param DataFilename		IN		The data filename, or URL
	 * @param DataGUID			OUT		Receives the GUID of the data
	 * @return  True if successful, false otherwise
	 */
	static bool GetGUIDFromFilename(const FString& DataFilename, FGuid& DataGUID);

	/**
	 * Helper function to uncompress chunk file data. Can be called without knowing if needed and process will be just skipped.
	 * @param ChunkFileArray	IN OUT		The data array, should contain full chunk - header plus data. Will be overwritten with uncompressed chunk version.
	 * @return		true if no errors occurred and the data is not corrupted
	 */
	static bool UncompressChunkFile(TArray<uint8>& ChunkFileArray);

	/**
	 * Helper function to uncompress file part data. Can be called without knowing if needed and process will be just skipped.
	 * @param DataFileArray	IN OUT		The data array, should contain full file - header plus data. Will be overwritten with uncompressed data version.
	 * @param OutHeader		OUT			Optional ptr to a header to receive the header from the file if you need it anyway
	 * @return		true if no errors occurred and the data is not corrupted
	 */
	static bool UncompressFileDataFile(TArray< uint8 >& DataFileArray, FChunkHeader* OutHeader = nullptr);

	/**
	 * Checks a file against SHA1 hashes. The function takes two so that it can return no match, match with Hash1, or match with Hash2, that way we can check the file for being the same as an old manifest or new manifest
	 * NOTE: This function is blocking and will not return until finished. Don't run on main thread.
	 * @param FileToVerify			IN		The file to analyze.
	 * @param Hash1					IN		A Hash to match against the file
	 * @param Hash2					IN		A second Hash to match against the file
	 * @param ProgressDelegate		IN		Delegate to receive progress updates in the form of a float range 0.0f to 1.0f
	 * @param ShouldPauseDelegate	IN		Delegate that returns a bool, which if true will pause the process
	 * @param TimeSpentPaused		OUT		The amount of time we were paused for, in seconds.
	 * @return		0 if no match, 1 for match with Hash1, and 2 for match with Hash2
	 */
	static uint8 VerifyFile(const FString& FileToVerify, const FSHAHashData& Hash1, const FSHAHashData& Hash2, FBuildPatchFloatDelegate ProgressDelegate, FBuildPatchBoolRetDelegate ShouldPauseDelegate, double& TimeSpentPaused);

	/**
	 * Checks a file against SHA1 hashes. The function takes two so that it can return no match, match with Hash1, or match with Hash2, that way we can check the file for being the same as an old manifest or new manifest
	 * NOTE: This function is blocking and will not return until finished. Don't run on main thread. This allows the above function to be easily called without delegates
	 * @param FileToVerify			IN		The file to analyze.
	 * @param Hash1					IN		A Hash to match against the file
	 * @param Hash2					IN		A second Hash to match against the file
	 * @return		0 if no match, 1 for match with Hash1, and 2 for match with Hash2
	 */
	static uint8 VerifyFile(const FString& FileToVerify, const FSHAHashData& Hash1, const FSHAHashData& Hash2);

	/**
	 * Helper function to verify the integrity of a chunk file that is in memory
	 * @param ChunkFileArray	The data array.
	 * @param bQuickCheck		If true, will only check header and file size, not data hash.
	 * @return		true if no errors occurred and the data is not corrupted
	 */
	static bool VerifyChunkFile(const TArray<uint8>& ChunkFileArray, bool bQuickCheck = false);

private:

	/**
	 * Internal function to verify the integrity of a chunk file called by the various public versions of the function
	 * @param ChunkFileData		The archive to test data on
	 * @param bQuickCheck		If true, will only check header and file size, not data hash.
	 * @return		true if no errors occurred and the data is not corrupted
	 */
	static bool VerifyChunkFile(FArchive& ChunkFileData, bool bQuickCheck = false);
};
