#include "ecrt.h"



// Global Functions Pointers

LIB_EXPORT C(GlobalFunction) * FUNCTION(qsortr);
LIB_EXPORT C(GlobalFunction) * FUNCTION(qsortrx);
LIB_EXPORT C(GlobalFunction) * FUNCTION(archiveOpen);
LIB_EXPORT C(GlobalFunction) * FUNCTION(archiveQuerySize);
LIB_EXPORT C(GlobalFunction) * FUNCTION(changeWorkingDir);
LIB_EXPORT C(GlobalFunction) * FUNCTION(copySystemPath);
LIB_EXPORT C(GlobalFunction) * FUNCTION(copyUnixPath);
LIB_EXPORT C(GlobalFunction) * FUNCTION(createTemporaryDir);
LIB_EXPORT C(GlobalFunction) * FUNCTION(createTemporaryFile);
LIB_EXPORT C(GlobalFunction) * FUNCTION(deleteFile);
LIB_EXPORT C(GlobalFunction) * FUNCTION(dualPipeOpen);
LIB_EXPORT C(GlobalFunction) * FUNCTION(dualPipeOpenEnv);
LIB_EXPORT C(GlobalFunction) * FUNCTION(dualPipeOpenEnvf);
LIB_EXPORT C(GlobalFunction) * FUNCTION(dualPipeOpenf);
LIB_EXPORT C(GlobalFunction) * FUNCTION(dumpErrors);
LIB_EXPORT C(GlobalFunction) * FUNCTION(execute);
LIB_EXPORT C(GlobalFunction) * FUNCTION(executeEnv);
LIB_EXPORT C(GlobalFunction) * FUNCTION(executeWait);
LIB_EXPORT C(GlobalFunction) * FUNCTION(fileExists);
LIB_EXPORT C(GlobalFunction) * FUNCTION(fileFixCase);
LIB_EXPORT C(GlobalFunction) * FUNCTION(fileGetSize);
LIB_EXPORT C(GlobalFunction) * FUNCTION(fileGetStats);
LIB_EXPORT C(GlobalFunction) * FUNCTION(fileOpen);
LIB_EXPORT C(GlobalFunction) * FUNCTION(fileOpenBuffered);
LIB_EXPORT C(GlobalFunction) * FUNCTION(fileSetAttribs);
LIB_EXPORT C(GlobalFunction) * FUNCTION(fileSetTime);
LIB_EXPORT C(GlobalFunction) * FUNCTION(fileTruncate);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getEnvironment);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getFreeSpace);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getLastErrorCode);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getSlashPathBuffer);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getSystemPathBuffer);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getWorkingDir);
LIB_EXPORT C(GlobalFunction) * FUNCTION(__e_log);
LIB_EXPORT C(GlobalFunction) * FUNCTION(logErrorCode);
LIB_EXPORT C(GlobalFunction) * FUNCTION(__e_logf);
LIB_EXPORT C(GlobalFunction) * FUNCTION(makeDir);
LIB_EXPORT C(GlobalFunction) * FUNCTION(makeSlashPath);
LIB_EXPORT C(GlobalFunction) * FUNCTION(makeSystemPath);
LIB_EXPORT C(GlobalFunction) * FUNCTION(moveFile);
LIB_EXPORT C(GlobalFunction) * FUNCTION(moveFileEx);
LIB_EXPORT C(GlobalFunction) * FUNCTION(removeDir);
LIB_EXPORT C(GlobalFunction) * FUNCTION(renameFile);
LIB_EXPORT C(GlobalFunction) * FUNCTION(resetError);
LIB_EXPORT C(GlobalFunction) * FUNCTION(setEnvironment);
LIB_EXPORT C(GlobalFunction) * FUNCTION(setErrorLevel);
LIB_EXPORT C(GlobalFunction) * FUNCTION(setLoggingMode);
LIB_EXPORT C(GlobalFunction) * FUNCTION(shellOpen);
LIB_EXPORT C(GlobalFunction) * FUNCTION(unsetEnvironment);
LIB_EXPORT C(GlobalFunction) * FUNCTION(debugBreakpoint);
LIB_EXPORT C(GlobalFunction) * FUNCTION(charMatchCategories);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getAlNum);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getCharCategory);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getCombiningClass);
LIB_EXPORT C(GlobalFunction) * FUNCTION(iSO8859_1toUTF8);
LIB_EXPORT C(GlobalFunction) * FUNCTION(uTF16BEtoUTF8Buffer);
LIB_EXPORT C(GlobalFunction) * FUNCTION(uTF16toUTF8);
LIB_EXPORT C(GlobalFunction) * FUNCTION(uTF16toUTF8Buffer);
LIB_EXPORT C(GlobalFunction) * FUNCTION(uTF32toUTF8Len);
LIB_EXPORT C(GlobalFunction) * FUNCTION(uTF8GetChar);
LIB_EXPORT C(GlobalFunction) * FUNCTION(uTF8Validate);
LIB_EXPORT C(GlobalFunction) * FUNCTION(uTF8toISO8859_1);
LIB_EXPORT C(GlobalFunction) * FUNCTION(uTF8toUTF16);
LIB_EXPORT C(GlobalFunction) * FUNCTION(uTF8toUTF16Buffer);
LIB_EXPORT C(GlobalFunction) * FUNCTION(uTF8toUTF16BufferLen);
LIB_EXPORT C(GlobalFunction) * FUNCTION(uTF8toUTF16Len);
LIB_EXPORT C(GlobalFunction) * FUNCTION(accenti);
LIB_EXPORT C(GlobalFunction) * FUNCTION(casei);
LIB_EXPORT C(GlobalFunction) * FUNCTION(encodeArrayToString);
LIB_EXPORT C(GlobalFunction) * FUNCTION(normalizeNFC);
LIB_EXPORT C(GlobalFunction) * FUNCTION(normalizeNFD);
LIB_EXPORT C(GlobalFunction) * FUNCTION(normalizeNFKC);
LIB_EXPORT C(GlobalFunction) * FUNCTION(normalizeNFKD);
LIB_EXPORT C(GlobalFunction) * FUNCTION(normalizeNFKDArray);
LIB_EXPORT C(GlobalFunction) * FUNCTION(normalizeUnicode);
LIB_EXPORT C(GlobalFunction) * FUNCTION(normalizeUnicodeArray);
LIB_EXPORT C(GlobalFunction) * FUNCTION(stripUnicodeCategory);
LIB_EXPORT C(GlobalFunction) * FUNCTION(printECONObject);
LIB_EXPORT C(GlobalFunction) * FUNCTION(printObjectNotationString);
LIB_EXPORT C(GlobalFunction) * FUNCTION(stringIndent);
LIB_EXPORT C(GlobalFunction) * FUNCTION(writeECONObject);
LIB_EXPORT C(GlobalFunction) * FUNCTION(writeJSONObject);
LIB_EXPORT C(GlobalFunction) * FUNCTION(writeJSONObject2);
LIB_EXPORT C(GlobalFunction) * FUNCTION(writeJSONObjectMapped);
LIB_EXPORT C(GlobalFunction) * FUNCTION(writeONString);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getCurrentThreadID);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getRandom);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getTime);
LIB_EXPORT C(GlobalFunction) * FUNCTION(randomSeed);
LIB_EXPORT C(GlobalFunction) * FUNCTION(__sleep);
LIB_EXPORT C(GlobalFunction) * FUNCTION(changeCh);
LIB_EXPORT C(GlobalFunction) * FUNCTION(changeChars);
LIB_EXPORT C(GlobalFunction) * FUNCTION(changeExtension);
LIB_EXPORT C(GlobalFunction) * FUNCTION(checkConsistency);
LIB_EXPORT C(GlobalFunction) * FUNCTION(checkMemory);
LIB_EXPORT C(GlobalFunction) * FUNCTION(copyBytes);
LIB_EXPORT C(GlobalFunction) * FUNCTION(copyBytesBy2);
LIB_EXPORT C(GlobalFunction) * FUNCTION(copyBytesBy4);
LIB_EXPORT C(GlobalFunction) * FUNCTION(copyString);
LIB_EXPORT C(GlobalFunction) * FUNCTION(escapeCString);
LIB_EXPORT C(GlobalFunction) * FUNCTION(fillBytes);
LIB_EXPORT C(GlobalFunction) * FUNCTION(fillBytesBy2);
LIB_EXPORT C(GlobalFunction) * FUNCTION(fillBytesBy4);
LIB_EXPORT C(GlobalFunction) * FUNCTION(floatFromString);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getActiveDesigner);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getExtension);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getHexValue);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getLastDirectory);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getRuntimePlatform);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getString);
LIB_EXPORT C(GlobalFunction) * FUNCTION(getValue);
LIB_EXPORT C(GlobalFunction) * FUNCTION(isPathInsideOf);
LIB_EXPORT C(GlobalFunction) * FUNCTION(locateModule);
LIB_EXPORT C(GlobalFunction) * FUNCTION(makePathRelative);
LIB_EXPORT C(GlobalFunction) * FUNCTION(moveBytes);
LIB_EXPORT C(GlobalFunction) * FUNCTION(pathCat);
LIB_EXPORT C(GlobalFunction) * FUNCTION(pathCatSlash);
LIB_EXPORT C(GlobalFunction) * FUNCTION(printx);
LIB_EXPORT C(GlobalFunction) * FUNCTION(printBigSize);
LIB_EXPORT C(GlobalFunction) * FUNCTION(printBuf);
LIB_EXPORT C(GlobalFunction) * FUNCTION(printLn);
LIB_EXPORT C(GlobalFunction) * FUNCTION(printLnBuf);
LIB_EXPORT C(GlobalFunction) * FUNCTION(printLnString);
LIB_EXPORT C(GlobalFunction) * FUNCTION(printSize);
LIB_EXPORT C(GlobalFunction) * FUNCTION(printStdArgsToBuffer);
LIB_EXPORT C(GlobalFunction) * FUNCTION(printString);
LIB_EXPORT C(GlobalFunction) * FUNCTION(rSearchString);
LIB_EXPORT C(GlobalFunction) * FUNCTION(repeatCh);
LIB_EXPORT C(GlobalFunction) * FUNCTION(searchString);
LIB_EXPORT C(GlobalFunction) * FUNCTION(setActiveDesigner);
LIB_EXPORT C(GlobalFunction) * FUNCTION(splitArchivePath);
LIB_EXPORT C(GlobalFunction) * FUNCTION(splitDirectory);
LIB_EXPORT C(GlobalFunction) * FUNCTION(stringLikePattern);
LIB_EXPORT C(GlobalFunction) * FUNCTION(stripChars);
LIB_EXPORT C(GlobalFunction) * FUNCTION(stripExtension);
LIB_EXPORT C(GlobalFunction) * FUNCTION(stripLastDirectory);
LIB_EXPORT C(GlobalFunction) * FUNCTION(stripQuotes);
LIB_EXPORT C(GlobalFunction) * FUNCTION(tokenize);
LIB_EXPORT C(GlobalFunction) * FUNCTION(tokenizeWith);
LIB_EXPORT C(GlobalFunction) * FUNCTION(trimLSpaces);
LIB_EXPORT C(GlobalFunction) * FUNCTION(trimRSpaces);
LIB_EXPORT C(GlobalFunction) * FUNCTION(unescapeCString);
LIB_EXPORT C(GlobalFunction) * FUNCTION(unescapeCStringLoose);
LIB_EXPORT C(GlobalFunction) * FUNCTION(eSystem_LockMem);
LIB_EXPORT C(GlobalFunction) * FUNCTION(eSystem_UnlockMem);
LIB_EXPORT C(GlobalFunction) * FUNCTION(ishexdigit);
LIB_EXPORT C(GlobalFunction) * FUNCTION(log2i);
LIB_EXPORT C(GlobalFunction) * FUNCTION(memswap);
LIB_EXPORT C(GlobalFunction) * FUNCTION(pow2i);
LIB_EXPORT C(GlobalFunction) * FUNCTION(queryMemInfo);
LIB_EXPORT C(GlobalFunction) * FUNCTION(strchrmax);



// Virtual Methods

LIB_EXPORT C(Method) * METHOD(class, onCompare);
LIB_EXPORT C(Method) * METHOD(class, onCopy);
LIB_EXPORT C(Method) * METHOD(class, onDisplay);
LIB_EXPORT C(Method) * METHOD(class, onEdit);
LIB_EXPORT C(Method) * METHOD(class, onFree);
LIB_EXPORT C(Method) * METHOD(class, onGetDataFromString);
LIB_EXPORT C(Method) * METHOD(class, onGetString);
LIB_EXPORT C(Method) * METHOD(class, onSaveEdit);
LIB_EXPORT C(Method) * METHOD(class, onSerialize);
LIB_EXPORT C(Method) * METHOD(class, onUnserialize);

LIB_EXPORT C(Method) * METHOD(double, inf);
LIB_EXPORT C(Method) * METHOD(double, nan);

LIB_EXPORT C(Method) * METHOD(float, inf);
LIB_EXPORT C(Method) * METHOD(float, nan);

LIB_EXPORT C(Method) * METHOD(Application, main);

LIB_EXPORT C(Method) * METHOD(Module, load);
LIB_EXPORT C(Method) * METHOD(Module, onLoad);
LIB_EXPORT C(Method) * METHOD(Module, onUnload);
LIB_EXPORT C(Method) * METHOD(Module, unload);

LIB_EXPORT C(Method) * METHOD(FieldValue, compareInt);
LIB_EXPORT C(Method) * METHOD(FieldValue, compareReal);
LIB_EXPORT C(Method) * METHOD(FieldValue, compareText);
LIB_EXPORT C(Method) * METHOD(FieldValue, formatArray);
LIB_EXPORT C(Method) * METHOD(FieldValue, formatFloat);
LIB_EXPORT C(Method) * METHOD(FieldValue, formatInteger);
LIB_EXPORT C(Method) * METHOD(FieldValue, formatMap);
LIB_EXPORT C(Method) * METHOD(FieldValue, getArrayOrMap);
LIB_EXPORT C(Method) * METHOD(FieldValue, stringify);

LIB_EXPORT C(Method) * METHOD(AVLNode, find);

LIB_EXPORT C(Method) * METHOD(BTNode, findPrefix);
LIB_EXPORT C(Method) * METHOD(BTNode, findString);

LIB_EXPORT C(Method) * METHOD(BinaryTree, add);
LIB_EXPORT C(Method) * METHOD(BinaryTree, check);
LIB_EXPORT C(Method) * METHOD(BinaryTree, compareInt);
LIB_EXPORT C(Method) * METHOD(BinaryTree, compareString);
LIB_EXPORT C(Method) * METHOD(BinaryTree, delete);
LIB_EXPORT C(Method) * METHOD(BinaryTree, find);
LIB_EXPORT C(Method) * METHOD(BinaryTree, findAll);
LIB_EXPORT C(Method) * METHOD(BinaryTree, findPrefix);
LIB_EXPORT C(Method) * METHOD(BinaryTree, findString);
LIB_EXPORT C(Method) * METHOD(BinaryTree, free);
LIB_EXPORT C(Method) * METHOD(BinaryTree, freeString);
LIB_EXPORT C(Method) * METHOD(BinaryTree, print);
LIB_EXPORT C(Method) * METHOD(BinaryTree, remove);

LIB_EXPORT C(Method) * METHOD(BuiltInContainer, add);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, copy);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, delete);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, find);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, free);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, freeIterator);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, getAtPosition);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, getCount);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, getData);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, getFirst);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, getLast);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, getNext);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, getPrev);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, insert);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, move);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, remove);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, removeAll);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, setData);
LIB_EXPORT C(Method) * METHOD(BuiltInContainer, sort);

LIB_EXPORT C(Method) * METHOD(Container, add);
LIB_EXPORT C(Method) * METHOD(Container, copy);
LIB_EXPORT C(Method) * METHOD(Container, delete);
LIB_EXPORT C(Method) * METHOD(Container, find);
LIB_EXPORT C(Method) * METHOD(Container, free);
LIB_EXPORT C(Method) * METHOD(Container, freeIterator);
LIB_EXPORT C(Method) * METHOD(Container, getAtPosition);
LIB_EXPORT C(Method) * METHOD(Container, getCount);
LIB_EXPORT C(Method) * METHOD(Container, getData);
LIB_EXPORT C(Method) * METHOD(Container, getFirst);
LIB_EXPORT C(Method) * METHOD(Container, getLast);
LIB_EXPORT C(Method) * METHOD(Container, getNext);
LIB_EXPORT C(Method) * METHOD(Container, getPrev);
LIB_EXPORT C(Method) * METHOD(Container, insert);
LIB_EXPORT C(Method) * METHOD(Container, move);
LIB_EXPORT C(Method) * METHOD(Container, remove);
LIB_EXPORT C(Method) * METHOD(Container, removeAll);
LIB_EXPORT C(Method) * METHOD(Container, setData);
LIB_EXPORT C(Method) * METHOD(Container, sort);
LIB_EXPORT C(Method) * METHOD(Container, takeOut);

LIB_EXPORT C(Method) * METHOD(CustomAVLTree, check);
LIB_EXPORT C(Method) * METHOD(CustomAVLTree, freeKey);

LIB_EXPORT C(Method) * METHOD(HashMap, removeIterating);
LIB_EXPORT C(Method) * METHOD(HashMap, resize);

LIB_EXPORT C(Method) * METHOD(Item, copy);

LIB_EXPORT C(Method) * METHOD(Iterator, find);
LIB_EXPORT C(Method) * METHOD(Iterator, free);
LIB_EXPORT C(Method) * METHOD(Iterator, getData);
LIB_EXPORT C(Method) * METHOD(Iterator, index);
LIB_EXPORT C(Method) * METHOD(Iterator, next);
LIB_EXPORT C(Method) * METHOD(Iterator, prev);
LIB_EXPORT C(Method) * METHOD(Iterator, remove);
LIB_EXPORT C(Method) * METHOD(Iterator, setData);

LIB_EXPORT C(Method) * METHOD(LinkList, _Sort);

LIB_EXPORT C(Method) * METHOD(OldLink, free);

LIB_EXPORT C(Method) * METHOD(OldList, add);
LIB_EXPORT C(Method) * METHOD(OldList, addName);
LIB_EXPORT C(Method) * METHOD(OldList, clear);
LIB_EXPORT C(Method) * METHOD(OldList, copy);
LIB_EXPORT C(Method) * METHOD(OldList, delete);
LIB_EXPORT C(Method) * METHOD(OldList, findLink);
LIB_EXPORT C(Method) * METHOD(OldList, findName);
LIB_EXPORT C(Method) * METHOD(OldList, findNamedLink);
LIB_EXPORT C(Method) * METHOD(OldList, free);
LIB_EXPORT C(Method) * METHOD(OldList, insert);
LIB_EXPORT C(Method) * METHOD(OldList, move);
LIB_EXPORT C(Method) * METHOD(OldList, placeName);
LIB_EXPORT C(Method) * METHOD(OldList, remove);
LIB_EXPORT C(Method) * METHOD(OldList, removeAll);
LIB_EXPORT C(Method) * METHOD(OldList, sort);
LIB_EXPORT C(Method) * METHOD(OldList, swap);

LIB_EXPORT C(Method) * METHOD(Archive, clear);
LIB_EXPORT C(Method) * METHOD(Archive, fileExists);
LIB_EXPORT C(Method) * METHOD(Archive, fileOpen);
LIB_EXPORT C(Method) * METHOD(Archive, fileOpenAtPosition);
LIB_EXPORT C(Method) * METHOD(Archive, fileOpenCompressed);
LIB_EXPORT C(Method) * METHOD(Archive, openDirectory);
LIB_EXPORT C(Method) * METHOD(Archive, setBufferRead);
LIB_EXPORT C(Method) * METHOD(Archive, setBufferSize);

LIB_EXPORT C(Method) * METHOD(ArchiveDir, add);
LIB_EXPORT C(Method) * METHOD(ArchiveDir, addFromFile);
LIB_EXPORT C(Method) * METHOD(ArchiveDir, addFromFileAtPosition);
LIB_EXPORT C(Method) * METHOD(ArchiveDir, delete);
LIB_EXPORT C(Method) * METHOD(ArchiveDir, fileExists);
LIB_EXPORT C(Method) * METHOD(ArchiveDir, fileOpen);
LIB_EXPORT C(Method) * METHOD(ArchiveDir, move);
LIB_EXPORT C(Method) * METHOD(ArchiveDir, openDirectory);
LIB_EXPORT C(Method) * METHOD(ArchiveDir, rename);

LIB_EXPORT C(Method) * METHOD(DualPipe, getExitCode);
LIB_EXPORT C(Method) * METHOD(DualPipe, getLinePeek);
LIB_EXPORT C(Method) * METHOD(DualPipe, getProcessID);
LIB_EXPORT C(Method) * METHOD(DualPipe, peek);
LIB_EXPORT C(Method) * METHOD(DualPipe, terminate);
LIB_EXPORT C(Method) * METHOD(DualPipe, wait);

LIB_EXPORT C(Method) * METHOD(File, close);
LIB_EXPORT C(Method) * METHOD(File, closeInput);
LIB_EXPORT C(Method) * METHOD(File, closeOutput);
LIB_EXPORT C(Method) * METHOD(File, copyTo);
LIB_EXPORT C(Method) * METHOD(File, copyToFile);
LIB_EXPORT C(Method) * METHOD(File, eof);
LIB_EXPORT C(Method) * METHOD(File, flush);
LIB_EXPORT C(Method) * METHOD(File, getDouble);
LIB_EXPORT C(Method) * METHOD(File, getFloat);
LIB_EXPORT C(Method) * METHOD(File, getHexValue);
LIB_EXPORT C(Method) * METHOD(File, getLine);
LIB_EXPORT C(Method) * METHOD(File, getLineEx);
LIB_EXPORT C(Method) * METHOD(File, getSize);
LIB_EXPORT C(Method) * METHOD(File, getString);
LIB_EXPORT C(Method) * METHOD(File, getValue);
LIB_EXPORT C(Method) * METHOD(File, getc);
LIB_EXPORT C(Method) * METHOD(File, lock);
LIB_EXPORT C(Method) * METHOD(File, print);
LIB_EXPORT C(Method) * METHOD(File, printLn);
LIB_EXPORT C(Method) * METHOD(File, printf);
LIB_EXPORT C(Method) * METHOD(File, putc);
LIB_EXPORT C(Method) * METHOD(File, puts);
LIB_EXPORT C(Method) * METHOD(File, read);
LIB_EXPORT C(Method) * METHOD(File, seek);
LIB_EXPORT C(Method) * METHOD(File, tell);
LIB_EXPORT C(Method) * METHOD(File, truncate);
LIB_EXPORT C(Method) * METHOD(File, unlock);
LIB_EXPORT C(Method) * METHOD(File, write);

LIB_EXPORT C(Method) * METHOD(FileListing, find);
LIB_EXPORT C(Method) * METHOD(FileListing, stop);

LIB_EXPORT C(Method) * METHOD(FileMonitor, onDirNotify);
LIB_EXPORT C(Method) * METHOD(FileMonitor, onFileNotify);
LIB_EXPORT C(Method) * METHOD(FileMonitor, startMonitoring);
LIB_EXPORT C(Method) * METHOD(FileMonitor, stopMonitoring);

LIB_EXPORT C(Method) * METHOD(TempFile, stealBuffer);

LIB_EXPORT C(Method) * METHOD(GlobalAppSettings, getGlobalValue);
LIB_EXPORT C(Method) * METHOD(GlobalAppSettings, putGlobalValue);

LIB_EXPORT C(Method) * METHOD(GlobalSettings, close);
LIB_EXPORT C(Method) * METHOD(GlobalSettings, closeAndMonitor);
LIB_EXPORT C(Method) * METHOD(GlobalSettings, load);
LIB_EXPORT C(Method) * METHOD(GlobalSettings, onAskReloadSettings);
LIB_EXPORT C(Method) * METHOD(GlobalSettings, openAndLock);
LIB_EXPORT C(Method) * METHOD(GlobalSettings, save);

LIB_EXPORT C(Method) * METHOD(GlobalSettingsDriver, load);
LIB_EXPORT C(Method) * METHOD(GlobalSettingsDriver, save);

LIB_EXPORT C(Method) * METHOD(JSONParser, getObject);

LIB_EXPORT C(Method) * METHOD(Condition, signal);
LIB_EXPORT C(Method) * METHOD(Condition, wait);

LIB_EXPORT C(Method) * METHOD(Mutex, release);
LIB_EXPORT C(Method) * METHOD(Mutex, wait);

LIB_EXPORT C(Method) * METHOD(Semaphore, release);
LIB_EXPORT C(Method) * METHOD(Semaphore, tryWait);
LIB_EXPORT C(Method) * METHOD(Semaphore, wait);

LIB_EXPORT C(Method) * METHOD(Thread, create);
LIB_EXPORT C(Method) * METHOD(Thread, kill);
LIB_EXPORT C(Method) * METHOD(Thread, main);
LIB_EXPORT C(Method) * METHOD(Thread, setPriority);
LIB_EXPORT C(Method) * METHOD(Thread, wait);

LIB_EXPORT C(Method) * METHOD(Date, onGetStringEn);

LIB_EXPORT C(Method) * METHOD(DateTime, fixDayOfYear);
LIB_EXPORT C(Method) * METHOD(DateTime, getLocalTime);

LIB_EXPORT C(Method) * METHOD(Month, getNumDays);

LIB_EXPORT C(Method) * METHOD(Box, clip);
LIB_EXPORT C(Method) * METHOD(Box, clipOffset);
LIB_EXPORT C(Method) * METHOD(Box, isPointInside);
LIB_EXPORT C(Method) * METHOD(Box, overlap);

LIB_EXPORT C(Method) * METHOD(ClassDesignerBase, addObject);
LIB_EXPORT C(Method) * METHOD(ClassDesignerBase, createNew);
LIB_EXPORT C(Method) * METHOD(ClassDesignerBase, createObject);
LIB_EXPORT C(Method) * METHOD(ClassDesignerBase, destroyObject);
LIB_EXPORT C(Method) * METHOD(ClassDesignerBase, droppedObject);
LIB_EXPORT C(Method) * METHOD(ClassDesignerBase, fixProperty);
LIB_EXPORT C(Method) * METHOD(ClassDesignerBase, listToolBoxClasses);
LIB_EXPORT C(Method) * METHOD(ClassDesignerBase, postCreateObject);
LIB_EXPORT C(Method) * METHOD(ClassDesignerBase, prepareTestObject);
LIB_EXPORT C(Method) * METHOD(ClassDesignerBase, reset);
LIB_EXPORT C(Method) * METHOD(ClassDesignerBase, selectObject);

LIB_EXPORT C(Method) * METHOD(DesignerBase, addDefaultMethod);
LIB_EXPORT C(Method) * METHOD(DesignerBase, addToolBoxClass);
LIB_EXPORT C(Method) * METHOD(DesignerBase, codeAddObject);
LIB_EXPORT C(Method) * METHOD(DesignerBase, deleteObject);
LIB_EXPORT C(Method) * METHOD(DesignerBase, findObject);
LIB_EXPORT C(Method) * METHOD(DesignerBase, modifyCode);
LIB_EXPORT C(Method) * METHOD(DesignerBase, objectContainsCode);
LIB_EXPORT C(Method) * METHOD(DesignerBase, renameObject);
LIB_EXPORT C(Method) * METHOD(DesignerBase, selectObjectFromDesigner);
LIB_EXPORT C(Method) * METHOD(DesignerBase, sheetAddObject);
LIB_EXPORT C(Method) * METHOD(DesignerBase, updateProperties);

LIB_EXPORT C(Method) * METHOD(IOChannel, get);
LIB_EXPORT C(Method) * METHOD(IOChannel, put);
LIB_EXPORT C(Method) * METHOD(IOChannel, readData);
LIB_EXPORT C(Method) * METHOD(IOChannel, serialize);
LIB_EXPORT C(Method) * METHOD(IOChannel, unserialize);
LIB_EXPORT C(Method) * METHOD(IOChannel, writeData);

LIB_EXPORT C(Method) * METHOD(SerialBuffer, free);

LIB_EXPORT C(Method) * METHOD(ZString, concat);
LIB_EXPORT C(Method) * METHOD(ZString, concatf);
LIB_EXPORT C(Method) * METHOD(ZString, concatn);
LIB_EXPORT C(Method) * METHOD(ZString, concatx);
LIB_EXPORT C(Method) * METHOD(ZString, copy);
LIB_EXPORT C(Method) * METHOD(ZString, copyString);




// Methods Function Pointers


LIB_EXPORT double (* double_inf)(void);
LIB_EXPORT double (* double_nan)(void);

LIB_EXPORT float (* float_inf)(void);
LIB_EXPORT float (* float_nan)(void);



LIB_EXPORT int (* FieldValue_compareInt)(C(FieldValue) * __this, C(FieldValue) * other);
LIB_EXPORT int (* FieldValue_compareReal)(C(FieldValue) * __this, C(FieldValue) * other);
LIB_EXPORT int (* FieldValue_compareText)(C(FieldValue) * __this, C(FieldValue) * other);
LIB_EXPORT C(String) (* FieldValue_formatArray)(C(FieldValue) * __this, char * tempString, void * fieldData, C(ObjectNotationType) * onType);
LIB_EXPORT C(String) (* FieldValue_formatFloat)(C(FieldValue) * __this, char * stringOutput, C(bool) fixDot);
LIB_EXPORT C(String) (* FieldValue_formatInteger)(C(FieldValue) * __this, char * stringOutput);
LIB_EXPORT C(String) (* FieldValue_formatMap)(C(FieldValue) * __this, char * tempString, void * fieldData, C(ObjectNotationType) * onType);
LIB_EXPORT C(bool) (* FieldValue_getArrayOrMap)(const char * string, C(Class) * destClass, void ** destination);
LIB_EXPORT C(String) (* FieldValue_stringify)(C(FieldValue) * __this);

LIB_EXPORT thisclass(AVLNode *) (* AVLNode_find)(C(AVLNode) * __this, C(Class) * Tclass, TP(AVLNode, T) key);

LIB_EXPORT C(BTNode) * (* BTNode_findPrefix)(C(BTNode) * __this, const char * key);
LIB_EXPORT C(BTNode) * (* BTNode_findString)(C(BTNode) * __this, const char * key);

LIB_EXPORT C(bool) (* BinaryTree_add)(C(BinaryTree) * __this, C(BTNode) * node);
LIB_EXPORT C(bool) (* BinaryTree_check)(C(BinaryTree) * __this);
LIB_EXPORT int (* BinaryTree_compareInt)(C(BinaryTree) * __this, uintptr a, uintptr b);
LIB_EXPORT int (* BinaryTree_compareString)(C(BinaryTree) * __this, const char * a, const char * b);
LIB_EXPORT void (* BinaryTree_delete)(C(BinaryTree) * __this, C(BTNode) * node);
LIB_EXPORT C(BTNode) * (* BinaryTree_find)(C(BinaryTree) * __this, uintptr key);
LIB_EXPORT C(BTNode) * (* BinaryTree_findAll)(C(BinaryTree) * __this, uintptr key);
LIB_EXPORT C(BTNode) * (* BinaryTree_findPrefix)(C(BinaryTree) * __this, const char * key);
LIB_EXPORT C(BTNode) * (* BinaryTree_findString)(C(BinaryTree) * __this, const char * key);
LIB_EXPORT void (* BinaryTree_free)(C(BinaryTree) * __this);
LIB_EXPORT void (* BinaryTree_freeString)(char * string);
LIB_EXPORT char * (* BinaryTree_print)(C(BinaryTree) * __this, char * output, C(TreePrintStyle) tps);
LIB_EXPORT void (* BinaryTree_remove)(C(BinaryTree) * __this, C(BTNode) * node);


LIB_EXPORT C(bool) (* Container_takeOut)(C(Container) __this, TP(Container, D) d);

LIB_EXPORT C(bool) (* CustomAVLTree_check)(C(CustomAVLTree) __this);
LIB_EXPORT void (* CustomAVLTree_freeKey)(C(CustomAVLTree) __this, C(AVLNode) * item);

LIB_EXPORT void (* HashMap_removeIterating)(C(HashMap) __this, C(IteratorPointer) * it);
LIB_EXPORT void (* HashMap_resize)(C(HashMap) __this, C(IteratorPointer) * movedEntry);

LIB_EXPORT void (* Item_copy)(C(Item) * __this, C(Item) * src, int size);

LIB_EXPORT C(bool) (* Iterator_find)(C(Iterator) * __this, TP(Iterator, T) value);
LIB_EXPORT void (* Iterator_free)(C(Iterator) * __this);
LIB_EXPORT TP(Iterator, T) (* Iterator_getData)(C(Iterator) * __this);
LIB_EXPORT C(bool) (* Iterator_index)(C(Iterator) * __this, TP(Iterator, IT) index, C(bool) create);
LIB_EXPORT C(bool) (* Iterator_next)(C(Iterator) * __this);
LIB_EXPORT C(bool) (* Iterator_prev)(C(Iterator) * __this);
LIB_EXPORT void (* Iterator_remove)(C(Iterator) * __this);
LIB_EXPORT C(bool) (* Iterator_setData)(C(Iterator) * __this, TP(Iterator, T) value);

LIB_EXPORT void (* LinkList__Sort)(C(LinkList) __this, C(bool) ascending, C(LinkList) * lists);

LIB_EXPORT void (* OldLink_free)(C(OldLink) * __this);

LIB_EXPORT void (* OldList_add)(C(OldList) * __this, void * item);
LIB_EXPORT C(bool) (* OldList_addName)(C(OldList) * __this, void * item);
LIB_EXPORT void (* OldList_clear)(C(OldList) * __this);
LIB_EXPORT void (* OldList_copy)(C(OldList) * __this, C(OldList) * src, int size, void (* copy)(void * dest, void * src));
LIB_EXPORT void (* OldList_delete)(C(OldList) * __this, void * item);
LIB_EXPORT C(OldLink) * (* OldList_findLink)(C(OldList) * __this, void * data);
LIB_EXPORT void * (* OldList_findName)(C(OldList) * __this, const char * name, C(bool) warn);
LIB_EXPORT void * (* OldList_findNamedLink)(C(OldList) * __this, const char * name, C(bool) warn);
LIB_EXPORT void (* OldList_free)(C(OldList) * __this, void (* freeFn)(void *));
LIB_EXPORT C(bool) (* OldList_insert)(C(OldList) * __this, void * prevItem, void * item);
LIB_EXPORT void (* OldList_move)(C(OldList) * __this, void * item, void * prevItem);
LIB_EXPORT C(bool) (* OldList_placeName)(C(OldList) * __this, const char * name, void ** place);
LIB_EXPORT void (* OldList_remove)(C(OldList) * __this, void * item);
LIB_EXPORT void (* OldList_removeAll)(C(OldList) * __this, void (* freeFn)(void *));
LIB_EXPORT void (* OldList_sort)(C(OldList) * __this, int (* compare)(void *, void *, void *), void * data);
LIB_EXPORT void (* OldList_swap)(C(OldList) * __this, void * item1, void * item2);


LIB_EXPORT C(bool) (* ArchiveDir_add)(C(ArchiveDir) __this, const char * name, const char * path, C(ArchiveAddMode) addMode, int compression, int * ratio, uint * newPosition);

LIB_EXPORT int (* DualPipe_getExitCode)(C(DualPipe) __this);
LIB_EXPORT C(bool) (* DualPipe_getLinePeek)(C(DualPipe) __this, char * s, int max, int * charsRead);
LIB_EXPORT int (* DualPipe_getProcessID)(C(DualPipe) __this);
LIB_EXPORT C(bool) (* DualPipe_peek)(C(DualPipe) __this);
LIB_EXPORT void (* DualPipe_terminate)(C(DualPipe) __this);
LIB_EXPORT void (* DualPipe_wait)(C(DualPipe) __this);

LIB_EXPORT C(bool) (* File_copyTo)(C(File) __this, const char * outputFileName);
LIB_EXPORT C(bool) (* File_copyToFile)(C(File) __this, C(File) f);
LIB_EXPORT C(bool) (* File_flush)(C(File) __this);
LIB_EXPORT double (* File_getDouble)(C(File) __this);
LIB_EXPORT float (* File_getFloat)(C(File) __this);
LIB_EXPORT uint (* File_getHexValue)(C(File) __this);
LIB_EXPORT C(bool) (* File_getLine)(C(File) __this, char * s, int max);
LIB_EXPORT int (* File_getLineEx)(C(File) __this, char * s, int max, C(bool) * hasNewLineChar);
LIB_EXPORT C(bool) (* File_getString)(C(File) __this, char * string, int max);
LIB_EXPORT int (* File_getValue)(C(File) __this);
LIB_EXPORT void (* File_print)(C(File) __this, typed_object_class_ptr class_object, const void * object, ...);
LIB_EXPORT void (* File_printLn)(C(File) __this, typed_object_class_ptr class_object, const void * object, ...);
LIB_EXPORT int (* File_printf)(C(File) __this, const char * format, ...);

LIB_EXPORT C(bool) (* FileListing_find)(C(FileListing) * __this);
LIB_EXPORT void (* FileListing_stop)(C(FileListing) * __this);

LIB_EXPORT void (* FileMonitor_startMonitoring)(C(FileMonitor) __this);
LIB_EXPORT void (* FileMonitor_stopMonitoring)(C(FileMonitor) __this);

LIB_EXPORT byte * (* TempFile_stealBuffer)(C(TempFile) __this);

LIB_EXPORT C(bool) (* GlobalAppSettings_getGlobalValue)(C(GlobalAppSettings) __this, const char * section, const char * name, C(GlobalSettingType) type, void * value);
LIB_EXPORT C(bool) (* GlobalAppSettings_putGlobalValue)(C(GlobalAppSettings) __this, const char * section, const char * name, C(GlobalSettingType) type, const void * value);

LIB_EXPORT void (* GlobalSettings_close)(C(GlobalSettings) __this);
LIB_EXPORT void (* GlobalSettings_closeAndMonitor)(C(GlobalSettings) __this);
LIB_EXPORT C(bool) (* GlobalSettings_openAndLock)(C(GlobalSettings) __this, C(FileSize) * fileSize);


LIB_EXPORT C(JSONResult) (* JSONParser_getObject)(C(JSONParser) __this, C(Class) * objectType, void ** object);

LIB_EXPORT void (* Condition_signal)(C(Condition) * __this);
LIB_EXPORT void (* Condition_wait)(C(Condition) * __this, C(Mutex) * mutex);

LIB_EXPORT void (* Mutex_release)(C(Mutex) * __this);
LIB_EXPORT void (* Mutex_wait)(C(Mutex) * __this);

LIB_EXPORT void (* Semaphore_release)(C(Semaphore) * __this);
LIB_EXPORT C(bool) (* Semaphore_tryWait)(C(Semaphore) * __this);
LIB_EXPORT void (* Semaphore_wait)(C(Semaphore) * __this);

LIB_EXPORT void (* Thread_create)(C(Thread) __this);
LIB_EXPORT void (* Thread_kill)(C(Thread) __this);
LIB_EXPORT void (* Thread_setPriority)(C(Thread) __this, C(ThreadPriority) priority);
LIB_EXPORT void (* Thread_wait)(C(Thread) __this);

LIB_EXPORT const char * (* Date_onGetStringEn)(C(Date) * __this, char * stringOutput, void * fieldData, C(ObjectNotationType) * onType);

LIB_EXPORT C(bool) (* DateTime_fixDayOfYear)(C(DateTime) * __this);
LIB_EXPORT C(bool) (* DateTime_getLocalTime)(C(DateTime) * __this);

LIB_EXPORT int (* Month_getNumDays)(C(Month) __this, int year);

LIB_EXPORT void (* Box_clip)(C(Box) * __this, C(Box) * against);
LIB_EXPORT void (* Box_clipOffset)(C(Box) * __this, C(Box) * against, int x, int y);
LIB_EXPORT C(bool) (* Box_isPointInside)(C(Box) * __this, C(Point) * point);
LIB_EXPORT C(bool) (* Box_overlap)(C(Box) * __this, C(Box) * box);



LIB_EXPORT void (* IOChannel_get)(C(IOChannel) __this, typed_object_class_ptr class_data, void * data);
LIB_EXPORT void (* IOChannel_put)(C(IOChannel) __this, typed_object_class_ptr class_data, void * data);
LIB_EXPORT void (* IOChannel_serialize)(C(IOChannel) __this, typed_object_class_ptr class_data, void * data);
LIB_EXPORT void (* IOChannel_unserialize)(C(IOChannel) __this, typed_object_class_ptr class_data, void * data);

LIB_EXPORT void (* SerialBuffer_free)(C(SerialBuffer) __this);

LIB_EXPORT void (* ZString_concat)(C(ZString) __this, C(ZString) s);
LIB_EXPORT void (* ZString_concatf)(C(ZString) __this, const char * format, ...);
LIB_EXPORT void (* ZString_concatn)(C(ZString) __this, C(ZString) s, int l);
LIB_EXPORT void (* ZString_concatx)(C(ZString) __this, typed_object_class_ptr class_object, const void * object, ...);
LIB_EXPORT void (* ZString_copy)(C(ZString) __this, C(ZString) s);
LIB_EXPORT void (* ZString_copyString)(C(ZString) __this, const char * value, int newLen);




LIB_EXPORT C(Property) * PROPERTY(double, isNan);
LIB_EXPORT C(bool) (* double_get_isNan)(const double d);

LIB_EXPORT C(Property) * PROPERTY(double, isInf);
LIB_EXPORT C(bool) (* double_get_isInf)(const double d);

LIB_EXPORT C(Property) * PROPERTY(double, signBit);
LIB_EXPORT int (* double_get_signBit)(const double d);

LIB_EXPORT C(Property) * PROPERTY(float, isNan);
LIB_EXPORT C(bool) (* float_get_isNan)(const float f);

LIB_EXPORT C(Property) * PROPERTY(float, isInf);
LIB_EXPORT C(bool) (* float_get_isInf)(const float f);

LIB_EXPORT C(Property) * PROPERTY(float, signBit);
LIB_EXPORT int (* float_get_signBit)(const float f);

LIB_EXPORT C(Property) * PROPERTY(AVLNode, prev);
LIB_EXPORT thisclass(AVLNode *) (* AVLNode_get_prev)(const C(AVLNode) * a);

LIB_EXPORT C(Property) * PROPERTY(AVLNode, next);
LIB_EXPORT thisclass(AVLNode *) (* AVLNode_get_next)(const C(AVLNode) * a);

LIB_EXPORT C(Property) * PROPERTY(AVLNode, minimum);
LIB_EXPORT thisclass(AVLNode *) (* AVLNode_get_minimum)(const C(AVLNode) * a);

LIB_EXPORT C(Property) * PROPERTY(AVLNode, maximum);
LIB_EXPORT thisclass(AVLNode *) (* AVLNode_get_maximum)(const C(AVLNode) * a);

LIB_EXPORT C(Property) * PROPERTY(AVLNode, count);
LIB_EXPORT int (* AVLNode_get_count)(const C(AVLNode) * a);

LIB_EXPORT C(Property) * PROPERTY(AVLNode, depthProp);
LIB_EXPORT int (* AVLNode_get_depthProp)(const C(AVLNode) * a);

LIB_EXPORT C(Property) * PROPERTY(Array, size);
LIB_EXPORT void (* Array_set_size)(const C(Array) a, uint value);
LIB_EXPORT uint (* Array_get_size)(const C(Array) a);

LIB_EXPORT C(Property) * PROPERTY(Array, minAllocSize);
LIB_EXPORT void (* Array_set_minAllocSize)(const C(Array) a, uint value);
LIB_EXPORT uint (* Array_get_minAllocSize)(const C(Array) a);

LIB_EXPORT C(Property) * PROPERTY(BTNode, prev);
LIB_EXPORT C(BTNode) * (* BTNode_get_prev)(const C(BTNode) * b);

LIB_EXPORT C(Property) * PROPERTY(BTNode, next);
LIB_EXPORT C(BTNode) * (* BTNode_get_next)(const C(BTNode) * b);

LIB_EXPORT C(Property) * PROPERTY(BTNode, minimum);
LIB_EXPORT C(BTNode) * (* BTNode_get_minimum)(const C(BTNode) * b);

LIB_EXPORT C(Property) * PROPERTY(BTNode, maximum);
LIB_EXPORT C(BTNode) * (* BTNode_get_maximum)(const C(BTNode) * b);

LIB_EXPORT C(Property) * PROPERTY(BTNode, count);
LIB_EXPORT int (* BTNode_get_count)(const C(BTNode) * b);

LIB_EXPORT C(Property) * PROPERTY(BTNode, depthProp);
LIB_EXPORT int (* BTNode_get_depthProp)(const C(BTNode) * b);

LIB_EXPORT C(Property) * PROPERTY(BinaryTree, first);
LIB_EXPORT C(BTNode) * (* BinaryTree_get_first)(const C(BinaryTree) * b);

LIB_EXPORT C(Property) * PROPERTY(BinaryTree, last);
LIB_EXPORT C(BTNode) * (* BinaryTree_get_last)(const C(BinaryTree) * b);

LIB_EXPORT C(Property) * PROPERTY(BuiltInContainer, Container);
LIB_EXPORT C(Container) (* BuiltInContainer_to_Container)(const C(BuiltInContainer) * b);

LIB_EXPORT C(Property) * PROPERTY(Container, copySrc);
LIB_EXPORT void (* Container_set_copySrc)(const C(Container) c, C(Container) value);

LIB_EXPORT C(Property) * PROPERTY(Container, firstIterator);
LIB_EXPORT void (* Container_get_firstIterator)(const C(Container) c, C(Iterator) * value);

LIB_EXPORT C(Property) * PROPERTY(Container, lastIterator);
LIB_EXPORT void (* Container_get_lastIterator)(const C(Container) c, C(Iterator) * value);

LIB_EXPORT C(Property) * PROPERTY(HashMap, count);
LIB_EXPORT int (* HashMap_get_count)(const C(HashMap) h);

LIB_EXPORT C(Property) * PROPERTY(HashMap, initSize);
LIB_EXPORT void (* HashMap_set_initSize)(const C(HashMap) h, int value);

LIB_EXPORT C(Property) * PROPERTY(HashMapIterator, map);
LIB_EXPORT void (* HashMapIterator_set_map)(const C(HashMapIterator) * h, C(HashMap) value);
LIB_EXPORT C(HashMap) (* HashMapIterator_get_map)(const C(HashMapIterator) * h);

LIB_EXPORT C(Property) * PROPERTY(HashMapIterator, key);
LIB_EXPORT TP(HashMapIterator, KT) (* HashMapIterator_get_key)(const C(HashMapIterator) * h);

LIB_EXPORT C(Property) * PROPERTY(HashMapIterator, value);
LIB_EXPORT void (* HashMapIterator_set_value)(const C(HashMapIterator) * h, TP(HashMapIterator, VT) value);
LIB_EXPORT TP(HashMapIterator, VT) (* HashMapIterator_get_value)(const C(HashMapIterator) * h);

LIB_EXPORT C(Property) * PROPERTY(HashTable, initSize);
LIB_EXPORT void (* HashTable_set_initSize)(const C(HashTable) h, int value);

LIB_EXPORT C(Property) * PROPERTY(Iterator, data);
LIB_EXPORT void (* Iterator_set_data)(const C(Iterator) * i, TP(Iterator, T) value);
LIB_EXPORT TP(Iterator, T) (* Iterator_get_data)(const C(Iterator) * i);

LIB_EXPORT C(Property) * PROPERTY(Map, mapSrc);
LIB_EXPORT void (* Map_set_mapSrc)(const C(Map) m, C(Map) value);

LIB_EXPORT C(Property) * PROPERTY(MapIterator, map);
LIB_EXPORT void (* MapIterator_set_map)(const C(MapIterator) * m, C(Map) value);
LIB_EXPORT C(Map) (* MapIterator_get_map)(const C(MapIterator) * m);

LIB_EXPORT C(Property) * PROPERTY(MapIterator, key);
LIB_EXPORT TP(MapIterator, KT) (* MapIterator_get_key)(const C(MapIterator) * m);

LIB_EXPORT C(Property) * PROPERTY(MapIterator, value);
LIB_EXPORT void (* MapIterator_set_value)(const C(MapIterator) * m, TP(MapIterator, V) value);
LIB_EXPORT TP(MapIterator, V) (* MapIterator_get_value)(const C(MapIterator) * m);

LIB_EXPORT C(Property) * PROPERTY(MapNode, key);
LIB_EXPORT void (* MapNode_set_key)(const C(MapNode) * m, TP(MapNode, KT) value);
LIB_EXPORT TP(MapNode, KT) (* MapNode_get_key)(const C(MapNode) * m);

LIB_EXPORT C(Property) * PROPERTY(MapNode, value);
LIB_EXPORT void (* MapNode_set_value)(const C(MapNode) * m, TP(MapNode, V) value);
LIB_EXPORT TP(MapNode, V) (* MapNode_get_value)(const C(MapNode) * m);

LIB_EXPORT C(Property) * PROPERTY(MapNode, prev);
LIB_EXPORT thisclass(MapNode *) (* MapNode_get_prev)(const C(MapNode) * m);

LIB_EXPORT C(Property) * PROPERTY(MapNode, next);
LIB_EXPORT thisclass(MapNode *) (* MapNode_get_next)(const C(MapNode) * m);

LIB_EXPORT C(Property) * PROPERTY(MapNode, minimum);
LIB_EXPORT thisclass(MapNode *) (* MapNode_get_minimum)(const C(MapNode) * m);

LIB_EXPORT C(Property) * PROPERTY(MapNode, maximum);
LIB_EXPORT thisclass(MapNode *) (* MapNode_get_maximum)(const C(MapNode) * m);

LIB_EXPORT C(Property) * PROPERTY(Archive, totalSize);
LIB_EXPORT void (* Archive_set_totalSize)(const C(Archive) a, C(FileSize) value);
LIB_EXPORT C(FileSize) (* Archive_get_totalSize)(const C(Archive) a);

LIB_EXPORT C(Property) * PROPERTY(Archive, bufferSize);
LIB_EXPORT void (* Archive_set_bufferSize)(const C(Archive) a, uint value);

LIB_EXPORT C(Property) * PROPERTY(Archive, bufferRead);
LIB_EXPORT void (* Archive_set_bufferRead)(const C(Archive) a, uint value);

LIB_EXPORT C(Property) * PROPERTY(BufferedFile, handle);
LIB_EXPORT void (* BufferedFile_set_handle)(const C(BufferedFile) b, C(File) value);
LIB_EXPORT C(File) (* BufferedFile_get_handle)(const C(BufferedFile) b);

LIB_EXPORT C(Property) * PROPERTY(BufferedFile, bufferSize);
LIB_EXPORT void (* BufferedFile_set_bufferSize)(const C(BufferedFile) b, uintsize value);
LIB_EXPORT uintsize (* BufferedFile_get_bufferSize)(const C(BufferedFile) b);

LIB_EXPORT C(Property) * PROPERTY(BufferedFile, bufferRead);
LIB_EXPORT void (* BufferedFile_set_bufferRead)(const C(BufferedFile) b, uintsize value);
LIB_EXPORT uintsize (* BufferedFile_get_bufferRead)(const C(BufferedFile) b);

LIB_EXPORT C(Property) * PROPERTY(File, input);
LIB_EXPORT void (* File_set_input)(const C(File) f, void * value);
LIB_EXPORT void * (* File_get_input)(const C(File) f);

LIB_EXPORT C(Property) * PROPERTY(File, output);
LIB_EXPORT void (* File_set_output)(const C(File) f, void * value);
LIB_EXPORT void * (* File_get_output)(const C(File) f);

LIB_EXPORT C(Property) * PROPERTY(File, buffered);
LIB_EXPORT void (* File_set_buffered)(const C(File) f, C(bool) value);

LIB_EXPORT C(Property) * PROPERTY(File, eof);
LIB_EXPORT C(bool) (* File_get_eof)(const C(File) f);

LIB_EXPORT C(Property) * PROPERTY(FileListing, name);
LIB_EXPORT const char * (* FileListing_get_name)(const C(FileListing) * f);

LIB_EXPORT C(Property) * PROPERTY(FileListing, path);
LIB_EXPORT const char * (* FileListing_get_path)(const C(FileListing) * f);

LIB_EXPORT C(Property) * PROPERTY(FileListing, stats);
LIB_EXPORT void (* FileListing_get_stats)(const C(FileListing) * f, C(FileStats) * value);

LIB_EXPORT C(Property) * PROPERTY(FileMonitor, userData);
LIB_EXPORT void (* FileMonitor_set_userData)(const C(FileMonitor) f, void * value);

LIB_EXPORT C(Property) * PROPERTY(FileMonitor, fileChange);
LIB_EXPORT void (* FileMonitor_set_fileChange)(const C(FileMonitor) f, C(FileChange) value);

LIB_EXPORT C(Property) * PROPERTY(FileMonitor, fileName);
LIB_EXPORT void (* FileMonitor_set_fileName)(const C(FileMonitor) f, const char * value);
LIB_EXPORT const char * (* FileMonitor_get_fileName)(const C(FileMonitor) f);

LIB_EXPORT C(Property) * PROPERTY(FileMonitor, directoryName);
LIB_EXPORT void (* FileMonitor_set_directoryName)(const C(FileMonitor) f, const char * value);
LIB_EXPORT const char * (* FileMonitor_get_directoryName)(const C(FileMonitor) f);

LIB_EXPORT C(Property) * PROPERTY(TempFile, openMode);
LIB_EXPORT void (* TempFile_set_openMode)(const C(TempFile) t, C(FileOpenMode) value);
LIB_EXPORT C(FileOpenMode) (* TempFile_get_openMode)(const C(TempFile) t);

LIB_EXPORT C(Property) * PROPERTY(TempFile, buffer);
LIB_EXPORT void (* TempFile_set_buffer)(const C(TempFile) t, byte * value);
LIB_EXPORT byte * (* TempFile_get_buffer)(const C(TempFile) t);

LIB_EXPORT C(Property) * PROPERTY(TempFile, size);
LIB_EXPORT void (* TempFile_set_size)(const C(TempFile) t, uintsize value);
LIB_EXPORT uintsize (* TempFile_get_size)(const C(TempFile) t);

LIB_EXPORT C(Property) * PROPERTY(TempFile, allocated);
LIB_EXPORT void (* TempFile_set_allocated)(const C(TempFile) t, uintsize value);
LIB_EXPORT uintsize (* TempFile_get_allocated)(const C(TempFile) t);

LIB_EXPORT C(Property) * PROPERTY(GlobalSettings, settingsName);
LIB_EXPORT void (* GlobalSettings_set_settingsName)(const C(GlobalSettings) g, const char * value);
LIB_EXPORT const char * (* GlobalSettings_get_settingsName)(const C(GlobalSettings) g);

LIB_EXPORT C(Property) * PROPERTY(GlobalSettings, settingsExtension);
LIB_EXPORT void (* GlobalSettings_set_settingsExtension)(const C(GlobalSettings) g, const char * value);
LIB_EXPORT const char * (* GlobalSettings_get_settingsExtension)(const C(GlobalSettings) g);

LIB_EXPORT C(Property) * PROPERTY(GlobalSettings, settingsDirectory);
LIB_EXPORT void (* GlobalSettings_set_settingsDirectory)(const C(GlobalSettings) g, const char * value);
LIB_EXPORT const char * (* GlobalSettings_get_settingsDirectory)(const C(GlobalSettings) g);

LIB_EXPORT C(Property) * PROPERTY(GlobalSettings, settingsLocation);
LIB_EXPORT void (* GlobalSettings_set_settingsLocation)(const C(GlobalSettings) g, const char * value);
LIB_EXPORT const char * (* GlobalSettings_get_settingsLocation)(const C(GlobalSettings) g);

LIB_EXPORT C(Property) * PROPERTY(GlobalSettings, settingsFilePath);
LIB_EXPORT void (* GlobalSettings_set_settingsFilePath)(const C(GlobalSettings) g, const char * value);
LIB_EXPORT const char * (* GlobalSettings_get_settingsFilePath)(const C(GlobalSettings) g);

LIB_EXPORT C(Property) * PROPERTY(GlobalSettings, allowDefaultLocations);
LIB_EXPORT void (* GlobalSettings_set_allowDefaultLocations)(const C(GlobalSettings) g, C(bool) value);
LIB_EXPORT C(bool) (* GlobalSettings_get_allowDefaultLocations)(const C(GlobalSettings) g);

LIB_EXPORT C(Property) * PROPERTY(GlobalSettings, allUsers);
LIB_EXPORT void (* GlobalSettings_set_allUsers)(const C(GlobalSettings) g, C(bool) value);
LIB_EXPORT C(bool) (* GlobalSettings_get_allUsers)(const C(GlobalSettings) g);

LIB_EXPORT C(Property) * PROPERTY(GlobalSettings, portable);
LIB_EXPORT void (* GlobalSettings_set_portable)(const C(GlobalSettings) g, C(bool) value);
LIB_EXPORT C(bool) (* GlobalSettings_get_portable)(const C(GlobalSettings) g);

LIB_EXPORT C(Property) * PROPERTY(GlobalSettings, driver);
LIB_EXPORT void (* GlobalSettings_set_driver)(const C(GlobalSettings) g, constString value);
LIB_EXPORT constString (* GlobalSettings_get_driver)(const C(GlobalSettings) g);

LIB_EXPORT C(Property) * PROPERTY(GlobalSettings, isGlobalPath);
LIB_EXPORT C(bool) (* GlobalSettings_get_isGlobalPath)(const C(GlobalSettings) g);

LIB_EXPORT C(Property) * PROPERTY(JSONParser, debug);
LIB_EXPORT void (* JSONParser_set_debug)(const C(JSONParser) j, C(bool) value);
LIB_EXPORT C(bool) (* JSONParser_get_debug)(const C(JSONParser) j);

LIB_EXPORT C(Property) * PROPERTY(JSONParser, warnings);
LIB_EXPORT void (* JSONParser_set_warnings)(const C(JSONParser) j, C(bool) value);
LIB_EXPORT C(bool) (* JSONParser_get_warnings)(const C(JSONParser) j);

LIB_EXPORT C(Property) * PROPERTY(Condition, name);
LIB_EXPORT void (* Condition_set_name)(const C(Condition) * c, const char * value);
LIB_EXPORT const char * (* Condition_get_name)(const C(Condition) * c);

LIB_EXPORT C(Property) * PROPERTY(Mutex, lockCount);
LIB_EXPORT int (* Mutex_get_lockCount)(const C(Mutex) * m);

LIB_EXPORT C(Property) * PROPERTY(Mutex, owningThread);
LIB_EXPORT int64 (* Mutex_get_owningThread)(const C(Mutex) * m);

LIB_EXPORT C(Property) * PROPERTY(Semaphore, initCount);
LIB_EXPORT void (* Semaphore_set_initCount)(const C(Semaphore) * s, int value);
LIB_EXPORT int (* Semaphore_get_initCount)(const C(Semaphore) * s);

LIB_EXPORT C(Property) * PROPERTY(Semaphore, maxCount);
LIB_EXPORT void (* Semaphore_set_maxCount)(const C(Semaphore) * s, int value);
LIB_EXPORT int (* Semaphore_get_maxCount)(const C(Semaphore) * s);

LIB_EXPORT C(Property) * PROPERTY(Thread, created);
LIB_EXPORT C(bool) (* Thread_get_created)(const C(Thread) t);

LIB_EXPORT C(Property) * PROPERTY(Date, dayOfTheWeek);
LIB_EXPORT C(DayOfTheWeek) (* Date_get_dayOfTheWeek)(const C(Date) * d);

LIB_EXPORT C(Property) * PROPERTY(DateTime, global);
LIB_EXPORT void (* DateTime_set_global)(const C(DateTime) * d, const C(DateTime) * value);
LIB_EXPORT void (* DateTime_get_global)(const C(DateTime) * d, C(DateTime) * value);

LIB_EXPORT C(Property) * PROPERTY(DateTime, local);
LIB_EXPORT void (* DateTime_set_local)(const C(DateTime) * d, const C(DateTime) * value);
LIB_EXPORT void (* DateTime_get_local)(const C(DateTime) * d, C(DateTime) * value);

LIB_EXPORT C(Property) * PROPERTY(DateTime, daysSince1970);
LIB_EXPORT int64 (* DateTime_get_daysSince1970)(const C(DateTime) * d);

LIB_EXPORT C(Property) * PROPERTY(DateTime, SecSince1970);
LIB_EXPORT void (* DateTime_from_SecSince1970)(const C(DateTime) * d, C(SecSince1970) value);
LIB_EXPORT C(SecSince1970) (* DateTime_to_SecSince1970)(const C(DateTime) * d);

LIB_EXPORT C(Property) * PROPERTY(DateTime, Date);
LIB_EXPORT void (* DateTime_from_Date)(const C(DateTime) * d, const C(Date) * value);
LIB_EXPORT void (* DateTime_to_Date)(const C(DateTime) * d, C(Date) * value);

LIB_EXPORT C(Property) * PROPERTY(SecSince1970, global);
LIB_EXPORT C(SecSince1970) (* SecSince1970_get_global)(const int64 s);

LIB_EXPORT C(Property) * PROPERTY(SecSince1970, local);
LIB_EXPORT C(SecSince1970) (* SecSince1970_get_local)(const int64 s);



LIB_EXPORT C(Property) * PROPERTY(Box, width);
LIB_EXPORT void (* Box_set_width)(const C(Box) * b, int value);
LIB_EXPORT int (* Box_get_width)(const C(Box) * b);

LIB_EXPORT C(Property) * PROPERTY(Box, height);
LIB_EXPORT void (* Box_set_height)(const C(Box) * b, int value);
LIB_EXPORT int (* Box_get_height)(const C(Box) * b);

LIB_EXPORT C(Property) * PROPERTY(Centimeters, Meters);
LIB_EXPORT double (* Centimeters_from_Meters)(const C(Distance) meters);
LIB_EXPORT C(Distance) (* Centimeters_to_Meters)(const double centimeters);

LIB_EXPORT C(Property) * PROPERTY(Class, char_ptr);
LIB_EXPORT void (* Class_from_char_ptr)(const C(Class) * c, const char * value);
LIB_EXPORT const char * (* Class_to_char_ptr)(const C(Class) * c);

LIB_EXPORT C(Property) * PROPERTY(Degrees, Radians);
LIB_EXPORT double (* Degrees_from_Radians)(const C(Angle) radians);
LIB_EXPORT C(Angle) (* Degrees_to_Radians)(const double degrees);

LIB_EXPORT C(Property) * PROPERTY(DesignerBase, classDesigner);
LIB_EXPORT void (* DesignerBase_set_classDesigner)(const C(DesignerBase) d, C(ClassDesignerBase) value);
LIB_EXPORT C(ClassDesignerBase) (* DesignerBase_get_classDesigner)(const C(DesignerBase) d);

LIB_EXPORT C(Property) * PROPERTY(DesignerBase, objectClass);
LIB_EXPORT void (* DesignerBase_set_objectClass)(const C(DesignerBase) d, const char * value);
LIB_EXPORT const char * (* DesignerBase_get_objectClass)(const C(DesignerBase) d);

LIB_EXPORT C(Property) * PROPERTY(DesignerBase, isDragging);
LIB_EXPORT void (* DesignerBase_set_isDragging)(const C(DesignerBase) d, C(bool) value);
LIB_EXPORT C(bool) (* DesignerBase_get_isDragging)(const C(DesignerBase) d);

LIB_EXPORT C(Property) * PROPERTY(Feet, Meters);
LIB_EXPORT double (* Feet_from_Meters)(const C(Distance) meters);
LIB_EXPORT C(Distance) (* Feet_to_Meters)(const double feet);


LIB_EXPORT C(Property) * PROPERTY(Platform, char_ptr);
LIB_EXPORT C(Platform) (* Platform_from_char_ptr)(const char * c);
LIB_EXPORT const char * (* Platform_to_char_ptr)(const C(Platform) platform);


LIB_EXPORT C(Property) * PROPERTY(SerialBuffer, buffer);
LIB_EXPORT void (* SerialBuffer_set_buffer)(const C(SerialBuffer) s, byte * value);
LIB_EXPORT byte * (* SerialBuffer_get_buffer)(const C(SerialBuffer) s);

LIB_EXPORT C(Property) * PROPERTY(SerialBuffer, size);
LIB_EXPORT void (* SerialBuffer_set_size)(const C(SerialBuffer) s, uint value);
LIB_EXPORT uint (* SerialBuffer_get_size)(const C(SerialBuffer) s);

LIB_EXPORT C(Property) * PROPERTY(ZString, string);
LIB_EXPORT void (* ZString_set_string)(const C(ZString) z, const char * value);
LIB_EXPORT const char * (* ZString_get_string)(const C(ZString) z);

LIB_EXPORT C(Property) * PROPERTY(ZString, char_ptr);
LIB_EXPORT C(ZString) (* ZString_from_char_ptr)(const char * c);
LIB_EXPORT const char * (* ZString_to_char_ptr)(const C(ZString) z);

LIB_EXPORT C(Property) * PROPERTY(ZString, String);
LIB_EXPORT C(ZString) (* ZString_from_String)(const C(String) string);
LIB_EXPORT constString (* ZString_to_String)(const C(ZString) z);


// Properties




// Classes

// bitClass
LIB_EXPORT C(Class) * CO(FieldTypeEx);
LIB_EXPORT C(Class) * CO(ArchiveOpenFlags);
LIB_EXPORT C(Class) * CO(ErrorCode);
LIB_EXPORT C(Class) * CO(FileAttribs);
LIB_EXPORT C(Class) * CO(FileChange);
LIB_EXPORT C(Class) * CO(MoveFileOptions);
LIB_EXPORT C(Class) * CO(PipeOpenMode);
LIB_EXPORT C(Class) * CO(CharCategories);
LIB_EXPORT C(Class) * CO(UnicodeDecomposition);
LIB_EXPORT C(Class) * CO(JSONTypeOptions);
LIB_EXPORT C(Class) * CO(EscapeCStringOptions);
// enumClass
// LIB_EXPORT C(Class) * CO(bool);
LIB_EXPORT C(Class) * CO(FieldType);
LIB_EXPORT C(Class) * CO(FieldValueFormat);
LIB_EXPORT C(Class) * CO(TreePrintStyle);
LIB_EXPORT C(Class) * CO(ArchiveAddMode);
LIB_EXPORT C(Class) * CO(ErrorLevel);
LIB_EXPORT C(Class) * CO(FileLock);
LIB_EXPORT C(Class) * CO(FileOpenMode);
LIB_EXPORT C(Class) * CO(FileSeekMode);
LIB_EXPORT C(Class) * CO(GuiErrorCode);
LIB_EXPORT C(Class) * CO(LoggingMode);
LIB_EXPORT C(Class) * CO(SysErrorCode);
LIB_EXPORT C(Class) * CO(CharCategory);
LIB_EXPORT C(Class) * CO(PredefinedCharCategories);
LIB_EXPORT C(Class) * CO(GlobalSettingType);
LIB_EXPORT C(Class) * CO(JSONFirstLetterCapitalization);
LIB_EXPORT C(Class) * CO(JSONResult);
LIB_EXPORT C(Class) * CO(SetBool);
LIB_EXPORT C(Class) * CO(SettingsIOResult);
LIB_EXPORT C(Class) * CO(ThreadPriority);
LIB_EXPORT C(Class) * CO(DayOfTheWeek);
LIB_EXPORT C(Class) * CO(Month);
LIB_EXPORT C(Class) * CO(AccessMode);
LIB_EXPORT C(Class) * CO(BackSlashEscaping);
LIB_EXPORT C(Class) * CO(ClassType);
LIB_EXPORT C(Class) * CO(DataMemberType);
LIB_EXPORT C(Class) * CO(ImportType);
LIB_EXPORT C(Class) * CO(MethodType);
LIB_EXPORT C(Class) * CO(ObjectNotationType);
LIB_EXPORT C(Class) * CO(Platform);
LIB_EXPORT C(Class) * CO(StringAllocType);
LIB_EXPORT C(Class) * CO(TemplateMemberType);
LIB_EXPORT C(Class) * CO(TemplateParameterType);
// unitClass
LIB_EXPORT C(Class) * CO(unichar);
LIB_EXPORT C(Class) * CO(FileSize);
LIB_EXPORT C(Class) * CO(FileSize64);
LIB_EXPORT C(Class) * CO(SecSince1970);
LIB_EXPORT C(Class) * CO(Seconds);
LIB_EXPORT C(Class) * CO(Time);
LIB_EXPORT C(Class) * CO(TimeStamp);
LIB_EXPORT C(Class) * CO(TimeStamp32);
LIB_EXPORT C(Class) * CO(Angle);
LIB_EXPORT C(Class) * CO(Centimeters);
LIB_EXPORT C(Class) * CO(Degrees);
LIB_EXPORT C(Class) * CO(Distance);
LIB_EXPORT C(Class) * CO(Feet);
LIB_EXPORT C(Class) * CO(Meters);
LIB_EXPORT C(Class) * CO(MinMaxValue);
LIB_EXPORT C(Class) * CO(Radians);
// systemClass
LIB_EXPORT C(Class) * CO(byte);
LIB_EXPORT C(Class) * CO(char);
LIB_EXPORT C(Class) * CO(class);
LIB_EXPORT C(Class) * CO(double);
LIB_EXPORT C(Class) * CO(enum);
LIB_EXPORT C(Class) * CO(float);
LIB_EXPORT C(Class) * CO(int);
LIB_EXPORT C(Class) * CO(int64);
LIB_EXPORT C(Class) * CO(intptr);
LIB_EXPORT C(Class) * CO(intsize);
LIB_EXPORT C(Class) * CO(short);
LIB_EXPORT C(Class) * CO(struct);
LIB_EXPORT C(Class) * CO(uint);
LIB_EXPORT C(Class) * CO(uint16);
LIB_EXPORT C(Class) * CO(uint32);
LIB_EXPORT C(Class) * CO(uint64);
LIB_EXPORT C(Class) * CO(uintptr);
LIB_EXPORT C(Class) * CO(uintsize);
// structClass
LIB_EXPORT C(Class) * CO(FieldValue);
LIB_EXPORT C(Class) * CO(BinaryTree);
LIB_EXPORT C(Class) * CO(BuiltInContainer);
LIB_EXPORT C(Class) * CO(HashMapIterator);
LIB_EXPORT C(Class) * CO(Iterator);
LIB_EXPORT C(Class) * CO(Iterator);
LIB_EXPORT C(Class) * CO(Iterator);
LIB_EXPORT C(Class) * CO(Iterator);
LIB_EXPORT C(Class) * CO(LinkElement);
LIB_EXPORT C(Class) * CO(LinkElement);
LIB_EXPORT C(Class) * CO(MapIterator);
LIB_EXPORT C(Class) * CO(OldList);
LIB_EXPORT C(Class) * CO(StringBinaryTree);
LIB_EXPORT C(Class) * CO(FileListing);
LIB_EXPORT C(Class) * CO(FileStats);
LIB_EXPORT C(Class) * CO(Date);
LIB_EXPORT C(Class) * CO(DateTime);
LIB_EXPORT C(Class) * CO(Box);
LIB_EXPORT C(Class) * CO(ClassTemplateArgument);
LIB_EXPORT C(Class) * CO(DataValue);
LIB_EXPORT C(Class) * CO(NameSpace);
LIB_EXPORT C(Class) * CO(Point);
LIB_EXPORT C(Class) * CO(Pointd);
LIB_EXPORT C(Class) * CO(Pointf);
LIB_EXPORT C(Class) * CO(Size);
LIB_EXPORT C(Class) * CO(StaticString);
// noHeadClass
LIB_EXPORT C(Class) * CO(AVLNode);
LIB_EXPORT C(Class) * CO(AVLNode);
LIB_EXPORT C(Class) * CO(AVLNode);
LIB_EXPORT C(Class) * CO(BTNode);
LIB_EXPORT C(Class) * CO(Item);
LIB_EXPORT C(Class) * CO(IteratorPointer);
LIB_EXPORT C(Class) * CO(Link);
LIB_EXPORT C(Class) * CO(ListItem);
LIB_EXPORT C(Class) * CO(MapNode);
LIB_EXPORT C(Class) * CO(MapNode);
LIB_EXPORT C(Class) * CO(MapNode);
LIB_EXPORT C(Class) * CO(MapNode);
LIB_EXPORT C(Class) * CO(MapNode);
LIB_EXPORT C(Class) * CO(MapNode);
LIB_EXPORT C(Class) * CO(NamedItem);
LIB_EXPORT C(Class) * CO(NamedLink);
LIB_EXPORT C(Class) * CO(NamedLink64);
LIB_EXPORT C(Class) * CO(OldLink);
LIB_EXPORT C(Class) * CO(StringBTNode);
LIB_EXPORT C(Class) * CO(Condition);
LIB_EXPORT C(Class) * CO(Mutex);
LIB_EXPORT C(Class) * CO(Semaphore);
LIB_EXPORT C(Class) * CO(BTNamedLink);
LIB_EXPORT C(Class) * CO(BitMember);
LIB_EXPORT C(Class) * CO(Class);
LIB_EXPORT C(Class) * CO(ClassProperty);
LIB_EXPORT C(Class) * CO(ClassTemplateParameter);
LIB_EXPORT C(Class) * CO(DataMember);
LIB_EXPORT C(Class) * CO(DefinedExpression);
LIB_EXPORT C(Class) * CO(EnumClassData);
LIB_EXPORT C(Class) * CO(GlobalFunction);
LIB_EXPORT C(Class) * CO(Method);
LIB_EXPORT C(Class) * CO(ObjectInfo);
LIB_EXPORT C(Class) * CO(Property);
LIB_EXPORT C(Class) * CO(SubModule);
// normalClass
LIB_EXPORT C(Class) * CO(String);
LIB_EXPORT C(Class) * CO(Application);
LIB_EXPORT C(Class) * CO(Instance);
LIB_EXPORT C(Class) * CO(Module);
LIB_EXPORT C(Class) * CO(AVLTree);
LIB_EXPORT C(Class) * CO(Array);
LIB_EXPORT C(Class) * CO(Array);
LIB_EXPORT C(Class) * CO(Array);
LIB_EXPORT C(Class) * CO(Container);
LIB_EXPORT C(Class) * CO(Container);
LIB_EXPORT C(Class) * CO(Container);
LIB_EXPORT C(Class) * CO(Container);
LIB_EXPORT C(Class) * CO(Container);
LIB_EXPORT C(Class) * CO(Container);
LIB_EXPORT C(Class) * CO(Container);
LIB_EXPORT C(Class) * CO(CustomAVLTree);
LIB_EXPORT C(Class) * CO(CustomAVLTree);
LIB_EXPORT C(Class) * CO(CustomAVLTree);
LIB_EXPORT C(Class) * CO(HashMap);
LIB_EXPORT C(Class) * CO(HashMap);
LIB_EXPORT C(Class) * CO(HashTable);
LIB_EXPORT C(Class) * CO(LinkList);
LIB_EXPORT C(Class) * CO(LinkList);
LIB_EXPORT C(Class) * CO(List);
LIB_EXPORT C(Class) * CO(Map);
LIB_EXPORT C(Class) * CO(Map);
LIB_EXPORT C(Class) * CO(Map);
LIB_EXPORT C(Class) * CO(Map);
LIB_EXPORT C(Class) * CO(Map);
LIB_EXPORT C(Class) * CO(Archive);
LIB_EXPORT C(Class) * CO(ArchiveDir);
LIB_EXPORT C(Class) * CO(BufferedFile);
LIB_EXPORT C(Class) * CO(ConsoleFile);
LIB_EXPORT C(Class) * CO(DualPipe);
LIB_EXPORT C(Class) * CO(File);
LIB_EXPORT C(Class) * CO(FileMonitor);
LIB_EXPORT C(Class) * CO(TempFile);
LIB_EXPORT C(Class) * CO(ECONGlobalSettings);
LIB_EXPORT C(Class) * CO(ECONParser);
LIB_EXPORT C(Class) * CO(GlobalAppSettings);
LIB_EXPORT C(Class) * CO(GlobalSettings);
LIB_EXPORT C(Class) * CO(GlobalSettingsData);
LIB_EXPORT C(Class) * CO(GlobalSettingsDriver);
LIB_EXPORT C(Class) * CO(JSONGlobalSettings);
LIB_EXPORT C(Class) * CO(JSONParser);
LIB_EXPORT C(Class) * CO(OptionsMap);
LIB_EXPORT C(Class) * CO(Thread);
LIB_EXPORT C(Class) * CO(CIString);
LIB_EXPORT C(Class) * CO(ClassDesignerBase);
LIB_EXPORT C(Class) * CO(DesignerBase);
LIB_EXPORT C(Class) * CO(IOChannel);
LIB_EXPORT C(Class) * CO(SerialBuffer);
LIB_EXPORT C(Class) * CO(ZString);



// Virtual Method IDs

LIB_EXPORT int M_VTBLID(class, onCompare);
LIB_EXPORT int M_VTBLID(class, onCopy);
LIB_EXPORT int M_VTBLID(class, onDisplay);
LIB_EXPORT int M_VTBLID(class, onEdit);
LIB_EXPORT int M_VTBLID(class, onFree);
LIB_EXPORT int M_VTBLID(class, onGetDataFromString);
LIB_EXPORT int M_VTBLID(class, onGetString);
LIB_EXPORT int M_VTBLID(class, onSaveEdit);
LIB_EXPORT int M_VTBLID(class, onSerialize);
LIB_EXPORT int M_VTBLID(class, onUnserialize);

LIB_EXPORT int M_VTBLID(Application, main);

LIB_EXPORT int M_VTBLID(Module, onLoad);
LIB_EXPORT int M_VTBLID(Module, onUnload);

LIB_EXPORT int M_VTBLID(BuiltInContainer, add);
LIB_EXPORT int M_VTBLID(BuiltInContainer, copy);
LIB_EXPORT int M_VTBLID(BuiltInContainer, delete);
LIB_EXPORT int M_VTBLID(BuiltInContainer, find);
LIB_EXPORT int M_VTBLID(BuiltInContainer, free);
LIB_EXPORT int M_VTBLID(BuiltInContainer, freeIterator);
LIB_EXPORT int M_VTBLID(BuiltInContainer, getAtPosition);
LIB_EXPORT int M_VTBLID(BuiltInContainer, getCount);
LIB_EXPORT int M_VTBLID(BuiltInContainer, getData);
LIB_EXPORT int M_VTBLID(BuiltInContainer, getFirst);
LIB_EXPORT int M_VTBLID(BuiltInContainer, getLast);
LIB_EXPORT int M_VTBLID(BuiltInContainer, getNext);
LIB_EXPORT int M_VTBLID(BuiltInContainer, getPrev);
LIB_EXPORT int M_VTBLID(BuiltInContainer, insert);
LIB_EXPORT int M_VTBLID(BuiltInContainer, move);
LIB_EXPORT int M_VTBLID(BuiltInContainer, remove);
LIB_EXPORT int M_VTBLID(BuiltInContainer, removeAll);
LIB_EXPORT int M_VTBLID(BuiltInContainer, setData);
LIB_EXPORT int M_VTBLID(BuiltInContainer, sort);

LIB_EXPORT int M_VTBLID(Container, add);
LIB_EXPORT int M_VTBLID(Container, copy);
LIB_EXPORT int M_VTBLID(Container, delete);
LIB_EXPORT int M_VTBLID(Container, find);
LIB_EXPORT int M_VTBLID(Container, free);
LIB_EXPORT int M_VTBLID(Container, freeIterator);
LIB_EXPORT int M_VTBLID(Container, getAtPosition);
LIB_EXPORT int M_VTBLID(Container, getCount);
LIB_EXPORT int M_VTBLID(Container, getData);
LIB_EXPORT int M_VTBLID(Container, getFirst);
LIB_EXPORT int M_VTBLID(Container, getLast);
LIB_EXPORT int M_VTBLID(Container, getNext);
LIB_EXPORT int M_VTBLID(Container, getPrev);
LIB_EXPORT int M_VTBLID(Container, insert);
LIB_EXPORT int M_VTBLID(Container, move);
LIB_EXPORT int M_VTBLID(Container, remove);
LIB_EXPORT int M_VTBLID(Container, removeAll);
LIB_EXPORT int M_VTBLID(Container, setData);
LIB_EXPORT int M_VTBLID(Container, sort);

LIB_EXPORT int M_VTBLID(Archive, clear);
LIB_EXPORT int M_VTBLID(Archive, fileExists);
LIB_EXPORT int M_VTBLID(Archive, fileOpen);
LIB_EXPORT int M_VTBLID(Archive, fileOpenAtPosition);
LIB_EXPORT int M_VTBLID(Archive, fileOpenCompressed);
LIB_EXPORT int M_VTBLID(Archive, openDirectory);
LIB_EXPORT int M_VTBLID(Archive, setBufferRead);
LIB_EXPORT int M_VTBLID(Archive, setBufferSize);

LIB_EXPORT int M_VTBLID(ArchiveDir, addFromFile);
LIB_EXPORT int M_VTBLID(ArchiveDir, addFromFileAtPosition);
LIB_EXPORT int M_VTBLID(ArchiveDir, delete);
LIB_EXPORT int M_VTBLID(ArchiveDir, fileExists);
LIB_EXPORT int M_VTBLID(ArchiveDir, fileOpen);
LIB_EXPORT int M_VTBLID(ArchiveDir, move);
LIB_EXPORT int M_VTBLID(ArchiveDir, openDirectory);
LIB_EXPORT int M_VTBLID(ArchiveDir, rename);

LIB_EXPORT int M_VTBLID(File, close);
LIB_EXPORT int M_VTBLID(File, closeInput);
LIB_EXPORT int M_VTBLID(File, closeOutput);
LIB_EXPORT int M_VTBLID(File, eof);
LIB_EXPORT int M_VTBLID(File, getSize);
LIB_EXPORT int M_VTBLID(File, getc);
LIB_EXPORT int M_VTBLID(File, lock);
LIB_EXPORT int M_VTBLID(File, putc);
LIB_EXPORT int M_VTBLID(File, puts);
LIB_EXPORT int M_VTBLID(File, read);
LIB_EXPORT int M_VTBLID(File, seek);
LIB_EXPORT int M_VTBLID(File, tell);
LIB_EXPORT int M_VTBLID(File, truncate);
LIB_EXPORT int M_VTBLID(File, unlock);
LIB_EXPORT int M_VTBLID(File, write);

LIB_EXPORT int M_VTBLID(FileMonitor, onDirNotify);
LIB_EXPORT int M_VTBLID(FileMonitor, onFileNotify);

LIB_EXPORT int M_VTBLID(GlobalSettings, load);
LIB_EXPORT int M_VTBLID(GlobalSettings, onAskReloadSettings);
LIB_EXPORT int M_VTBLID(GlobalSettings, save);

LIB_EXPORT int M_VTBLID(GlobalSettingsDriver, load);
LIB_EXPORT int M_VTBLID(GlobalSettingsDriver, save);

LIB_EXPORT int M_VTBLID(Thread, main);

LIB_EXPORT int M_VTBLID(ClassDesignerBase, addObject);
LIB_EXPORT int M_VTBLID(ClassDesignerBase, createNew);
LIB_EXPORT int M_VTBLID(ClassDesignerBase, createObject);
LIB_EXPORT int M_VTBLID(ClassDesignerBase, destroyObject);
LIB_EXPORT int M_VTBLID(ClassDesignerBase, droppedObject);
LIB_EXPORT int M_VTBLID(ClassDesignerBase, fixProperty);
LIB_EXPORT int M_VTBLID(ClassDesignerBase, listToolBoxClasses);
LIB_EXPORT int M_VTBLID(ClassDesignerBase, postCreateObject);
LIB_EXPORT int M_VTBLID(ClassDesignerBase, prepareTestObject);
LIB_EXPORT int M_VTBLID(ClassDesignerBase, reset);
LIB_EXPORT int M_VTBLID(ClassDesignerBase, selectObject);

LIB_EXPORT int M_VTBLID(DesignerBase, addDefaultMethod);
LIB_EXPORT int M_VTBLID(DesignerBase, addToolBoxClass);
LIB_EXPORT int M_VTBLID(DesignerBase, codeAddObject);
LIB_EXPORT int M_VTBLID(DesignerBase, deleteObject);
LIB_EXPORT int M_VTBLID(DesignerBase, findObject);
LIB_EXPORT int M_VTBLID(DesignerBase, modifyCode);
LIB_EXPORT int M_VTBLID(DesignerBase, objectContainsCode);
LIB_EXPORT int M_VTBLID(DesignerBase, renameObject);
LIB_EXPORT int M_VTBLID(DesignerBase, selectObjectFromDesigner);
LIB_EXPORT int M_VTBLID(DesignerBase, sheetAddObject);
LIB_EXPORT int M_VTBLID(DesignerBase, updateProperties);

LIB_EXPORT int M_VTBLID(IOChannel, readData);
LIB_EXPORT int M_VTBLID(IOChannel, writeData);




// Global Functions

LIB_EXPORT void (* F(qsortr))(void * base, uintsize nel, uintsize width, int (* compare)(void * arg, const void * a, const void * b), void * arg);
LIB_EXPORT void (* F(qsortrx))(void * base, uintsize nel, uintsize width, int (* compare)(void * arg, const void * a, const void * b), int (* optCompareArgLast)(const void * a, const void * b, void * arg), void * arg, C(bool) deref, C(bool) ascending);
LIB_EXPORT C(Archive) (* F(archiveOpen))(const char * fileName, C(ArchiveOpenFlags) flags);
LIB_EXPORT C(bool) (* F(archiveQuerySize))(const char * fileName, C(FileSize) * size);
LIB_EXPORT C(bool) (* F(changeWorkingDir))(const char * buf);
LIB_EXPORT char * (* F(copySystemPath))(const char * p);
LIB_EXPORT char * (* F(copyUnixPath))(const char * p);
LIB_EXPORT void (* F(createTemporaryDir))(char * tempFileName, const char * _template);
LIB_EXPORT C(File) (* F(createTemporaryFile))(char * tempFileName, const char * _template);
LIB_EXPORT C(bool) (* F(deleteFile))(const char * fileName);
LIB_EXPORT C(DualPipe) (* F(dualPipeOpen))(C(PipeOpenMode) mode, const char * commandLine);
LIB_EXPORT C(DualPipe) (* F(dualPipeOpenEnv))(C(PipeOpenMode) mode, const char * env, const char * commandLine);
LIB_EXPORT C(DualPipe) (* F(dualPipeOpenEnvf))(C(PipeOpenMode) mode, const char * env, const char * command, ...);
LIB_EXPORT C(DualPipe) (* F(dualPipeOpenf))(C(PipeOpenMode) mode, const char * command, ...);
LIB_EXPORT void (* F(dumpErrors))(C(bool) display);
LIB_EXPORT C(bool) (* F(execute))(const char * command, ...);
LIB_EXPORT C(bool) (* F(executeEnv))(const char * env, const char * command, ...);
LIB_EXPORT C(bool) (* F(executeWait))(const char * command, ...);
LIB_EXPORT C(FileAttribs) (* F(fileExists))(const char * fileName);
LIB_EXPORT void (* F(fileFixCase))(char * file);
LIB_EXPORT C(bool) (* F(fileGetSize))(const char * fileName, C(FileSize) * size);
LIB_EXPORT C(bool) (* F(fileGetStats))(const char * fileName, C(FileStats) * stats);
LIB_EXPORT C(File) (* F(fileOpen))(const char * fileName, C(FileOpenMode) mode);
LIB_EXPORT C(BufferedFile) (* F(fileOpenBuffered))(const char * fileName, C(FileOpenMode) mode);
LIB_EXPORT C(bool) (* F(fileSetAttribs))(const char * fileName, C(FileAttribs) attribs);
LIB_EXPORT C(bool) (* F(fileSetTime))(const char * fileName, C(SecSince1970) created, C(SecSince1970) accessed, C(SecSince1970) modified);
LIB_EXPORT C(bool) (* F(fileTruncate))(const char * fileName, uint64 size);
LIB_EXPORT char * (* F(getEnvironment))(const char * envName, char * envValue, int max);
LIB_EXPORT void (* F(getFreeSpace))(const char * path, C(FileSize64) * size);
LIB_EXPORT uint (* F(getLastErrorCode))(void);
LIB_EXPORT char * (* F(getSlashPathBuffer))(char * d, const char * p);
LIB_EXPORT char * (* F(getSystemPathBuffer))(char * d, const char * p);
LIB_EXPORT char * (* F(getWorkingDir))(char * buf, int size);
LIB_EXPORT void (* F(__e_log))(const char * text);
LIB_EXPORT void (* F(logErrorCode))(C(ErrorCode) errorCode, const char * details);
LIB_EXPORT void (* F(__e_logf))(const char * format, ...);
LIB_EXPORT C(bool) (* F(makeDir))(const char * path);
LIB_EXPORT void (* F(makeSlashPath))(char * p);
LIB_EXPORT void (* F(makeSystemPath))(char * p);
LIB_EXPORT C(bool) (* F(moveFile))(const char * source, const char * dest);
LIB_EXPORT C(bool) (* F(moveFileEx))(const char * source, const char * dest, C(MoveFileOptions) options);
LIB_EXPORT C(bool) (* F(removeDir))(const char * path);
LIB_EXPORT C(bool) (* F(renameFile))(const char * oldName, const char * newName);
LIB_EXPORT void (* F(resetError))(void);
LIB_EXPORT void (* F(setEnvironment))(const char * envName, const char * envValue);
LIB_EXPORT void (* F(setErrorLevel))(C(ErrorLevel) level);
LIB_EXPORT void (* F(setLoggingMode))(C(LoggingMode) mode, void * where);
LIB_EXPORT C(bool) (* F(shellOpen))(const char * fileName, ...);
LIB_EXPORT void (* F(unsetEnvironment))(const char * envName);
LIB_EXPORT void (* F(debugBreakpoint))(void);
LIB_EXPORT C(bool) (* F(charMatchCategories))(unichar ch, C(CharCategories) categories);
LIB_EXPORT C(bool) (* F(getAlNum))(const char ** input, char * string, int max);
LIB_EXPORT C(CharCategory) (* F(getCharCategory))(unichar ch);
LIB_EXPORT uint (* F(getCombiningClass))(unichar ch);
LIB_EXPORT int (* F(iSO8859_1toUTF8))(const char * source, char * dest, int max);
LIB_EXPORT int (* F(uTF16BEtoUTF8Buffer))(const uint16 * source, byte * dest, int max);
LIB_EXPORT char * (* F(uTF16toUTF8))(const uint16 * source);
LIB_EXPORT int (* F(uTF16toUTF8Buffer))(const uint16 * source, char * dest, int max);
LIB_EXPORT int (* F(uTF32toUTF8Len))(const unichar * source, int count, char * dest, int max);
LIB_EXPORT unichar (* F(uTF8GetChar))(const char * string, int * numBytes);
LIB_EXPORT C(bool) (* F(uTF8Validate))(const char * source);
LIB_EXPORT int (* F(uTF8toISO8859_1))(const char * source, char * dest, int max);
LIB_EXPORT uint16 * (* F(uTF8toUTF16))(const char * source, int * wordCount);
LIB_EXPORT int (* F(uTF8toUTF16Buffer))(const char * source, uint16 * dest, int max);
LIB_EXPORT int (* F(uTF8toUTF16BufferLen))(const char * source, uint16 * dest, int max, int len);
LIB_EXPORT uint16 * (* F(uTF8toUTF16Len))(const char * source, int byteCount, int * wordCount);
LIB_EXPORT C(String) (* F(accenti))(constString string);
LIB_EXPORT C(String) (* F(casei))(constString string);
LIB_EXPORT C(String) (* F(encodeArrayToString))(C(Array) array);
LIB_EXPORT C(String) (* F(normalizeNFC))(constString string);
LIB_EXPORT C(String) (* F(normalizeNFD))(constString string);
LIB_EXPORT C(String) (* F(normalizeNFKC))(constString string);
LIB_EXPORT C(String) (* F(normalizeNFKD))(constString string);
LIB_EXPORT C(Array) (* F(normalizeNFKDArray))(constString string);
LIB_EXPORT C(String) (* F(normalizeUnicode))(constString string, C(UnicodeDecomposition) type, C(bool) compose);
LIB_EXPORT C(Array) (* F(normalizeUnicodeArray))(constString string, C(UnicodeDecomposition) type, C(bool) compose);
LIB_EXPORT C(String) (* F(stripUnicodeCategory))(constString string, C(CharCategory) c);
LIB_EXPORT C(String) (* F(printECONObject))(C(Class) * objectType, void * object, int indent);
LIB_EXPORT C(String) (* F(printObjectNotationString))(C(Class) * objectType, void * object, C(ObjectNotationType) onType, int indent, C(bool) indentFirst, C(JSONFirstLetterCapitalization) capitalize);
LIB_EXPORT C(String) (* F(stringIndent))(constString base, int nSpaces, C(bool) indentFirst);
LIB_EXPORT C(bool) (* F(writeECONObject))(C(File) f, C(Class) * objectType, void * object, int indent);
LIB_EXPORT C(bool) (* F(writeJSONObject))(C(File) f, C(Class) * objectType, void * object, int indent);
LIB_EXPORT C(bool) (* F(writeJSONObject2))(C(File) f, C(Class) * objectType, void * object, int indent, C(JSONFirstLetterCapitalization) capitalize);
LIB_EXPORT C(bool) (* F(writeJSONObjectMapped))(C(File) f, C(Class) * objectType, void * object, int indent, C(Map) stringMap);
LIB_EXPORT C(bool) (* F(writeONString))(C(File) f, constString s, C(bool) eCON, int indent);
LIB_EXPORT int64 (* F(getCurrentThreadID))(void);
LIB_EXPORT int (* F(getRandom))(int lo, int hi);
LIB_EXPORT C(Time) (* F(getTime))(void);
LIB_EXPORT void (* F(randomSeed))(uint seed);
LIB_EXPORT void (* F(__sleep))(C(Time) seconds);
LIB_EXPORT void (* F(changeCh))(char * string, char ch1, char ch2);
LIB_EXPORT void (* F(changeChars))(char * string, const char * chars, char alt);
LIB_EXPORT char * (* F(changeExtension))(const char * string, const char * ext, char * output);
LIB_EXPORT void (* F(checkConsistency))(void);
LIB_EXPORT void (* F(checkMemory))(void);
LIB_EXPORT void (* F(copyBytes))(void * dest, const void * source, uint64 count);
LIB_EXPORT void (* F(copyBytesBy2))(void * dest, const void * source, uint count);
LIB_EXPORT void (* F(copyBytesBy4))(void * dest, const void * source, uint count);
LIB_EXPORT char * (* F(copyString))(const char * string);
LIB_EXPORT int (* F(escapeCString))(C(String) outString, int bufferLen, constString s, C(EscapeCStringOptions) options);
LIB_EXPORT void (* F(fillBytes))(void * area, byte value, uint count);
LIB_EXPORT void (* F(fillBytesBy2))(void * area, uint16 value, uint count);
LIB_EXPORT void (* F(fillBytesBy4))(void * area, uint value, uint count);
LIB_EXPORT double (* F(floatFromString))(const char * string);
LIB_EXPORT C(DesignerBase) (* F(getActiveDesigner))(void);
LIB_EXPORT char * (* F(getExtension))(const char * string, char * output);
LIB_EXPORT uint (* F(getHexValue))(const char ** buffer);
LIB_EXPORT char * (* F(getLastDirectory))(const char * string, char * output);
LIB_EXPORT C(Platform) (* F(getRuntimePlatform))(void);
LIB_EXPORT C(bool) (* F(getString))(const char ** buffer, char * string, int max);
LIB_EXPORT int (* F(getValue))(const char ** buffer);
LIB_EXPORT C(bool) (* F(isPathInsideOf))(const char * path, const char * of);
LIB_EXPORT C(bool) (* F(locateModule))(const char * name, const char * fileName);
LIB_EXPORT char * (* F(makePathRelative))(const char * path, const char * to, char * destination);
LIB_EXPORT void (* F(moveBytes))(void * dest, const void * source, uint count);
LIB_EXPORT char * (* F(pathCat))(char * string, const char * addedPath);
LIB_EXPORT char * (* F(pathCatSlash))(char * string, const char * addedPath);
LIB_EXPORT void (* F(printx))(typed_object_class_ptr class_object, const void * object, ...);
LIB_EXPORT void (* F(printBigSize))(char * string, double size, int prec);
LIB_EXPORT int (* F(printBuf))(char * buffer, int maxLen, typed_object_class_ptr class_object, const void * object, ...);
LIB_EXPORT void (* F(printLn))(typed_object_class_ptr class_object, const void * object, ...);
LIB_EXPORT int (* F(printLnBuf))(char * buffer, int maxLen, typed_object_class_ptr class_object, const void * object, ...);
LIB_EXPORT char * (* F(printLnString))(typed_object_class_ptr class_object, const void * object, ...);
LIB_EXPORT void (* F(printSize))(char * string, uint64 size, int prec);
LIB_EXPORT int (* F(printStdArgsToBuffer))(char * buffer, int maxLen, typed_object_class_ptr class_object, const void * object, va_list args);
LIB_EXPORT char * (* F(printString))(typed_object_class_ptr class_object, const void * object, ...);
LIB_EXPORT char * (* F(rSearchString))(const char * buffer, const char * subStr, int maxLen, C(bool) matchCase, C(bool) matchWord);
LIB_EXPORT void (* F(repeatCh))(char * string, int count, char ch);
LIB_EXPORT char * (* F(searchString))(const char * buffer, int start, const char * subStr, C(bool) matchCase, C(bool) matchWord);
LIB_EXPORT void (* F(setActiveDesigner))(C(DesignerBase) designer);
LIB_EXPORT C(bool) (* F(splitArchivePath))(const char * fileName, char * archiveName, const char ** archiveFile);
LIB_EXPORT char * (* F(splitDirectory))(const char * string, char * part, char * rest);
LIB_EXPORT C(bool) (* F(stringLikePattern))(constString string, constString pattern);
LIB_EXPORT char * (* F(stripChars))(C(String) string, constString chars);
LIB_EXPORT C(bool) (* F(stripExtension))(char * string);
LIB_EXPORT char * (* F(stripLastDirectory))(const char * string, char * output);
LIB_EXPORT char * (* F(stripQuotes))(const char * string, char * output);
LIB_EXPORT int (* F(tokenize))(char * string, int maxTokens, char * tokens[], C(BackSlashEscaping) esc);
LIB_EXPORT int (* F(tokenizeWith))(char * string, int maxTokens, char * tokens[], const char * tokenizers, C(bool) escapeBackSlashes);
LIB_EXPORT char * (* F(trimLSpaces))(const char * string, char * output);
LIB_EXPORT char * (* F(trimRSpaces))(const char * string, char * output);
LIB_EXPORT int (* F(unescapeCString))(char * d, const char * s, int len);
LIB_EXPORT int (* F(unescapeCStringLoose))(char * d, const char * s, int len);
LIB_EXPORT void (* F(eSystem_LockMem))(void);
LIB_EXPORT void (* F(eSystem_UnlockMem))(void);
LIB_EXPORT C(bool) (* F(ishexdigit))(char x);
LIB_EXPORT uint (* F(log2i))(uint number);
LIB_EXPORT void (* F(memswap))(byte * a, byte * b, uint size);
LIB_EXPORT uint (* F(pow2i))(uint number);
LIB_EXPORT void (* F(queryMemInfo))(char * string);
LIB_EXPORT char * (* F(strchrmax))(const char * s, int c, int max);


LIB_EXPORT C(Application) ecrt_init(C(Module) fromModule, C(bool) obsolete, C(bool) guiApp, int argc, char * argv[])
{
#ifdef _DEBUG
   // printf("%s_init\n", "ecrt");
#endif

   if(!fromModule)
   {
      fromModule = eC_initApp(guiApp, argc, argv);
      if(fromModule) fromModule->_refCount++;
   }
   __thisModule = fromModule;
   if(fromModule)
   {
      C(Module) app = fromModule;
      C(Module) module = Module_load(fromModule, "ecrt", AccessMode_publicAccess);
      if(module)
      {
         // Set up all the CO(x) *, property, method, ...


         CO(String) = eC_findClass(app, "String");
         if(CO(String))
         {
         }
         CO(char) = eC_findClass(app, "char");
         CO(class) = eC_findClass(app, "class");
         if(CO(class))
         {
            METHOD(class, onCompare) = Class_findMethod(CO(class), "OnCompare", app);
            if(METHOD(class, onCompare))
               M_VTBLID(class, onCompare) = METHOD(class, onCompare)->vid;

            METHOD(class, onCopy) = Class_findMethod(CO(class), "OnCopy", app);
            if(METHOD(class, onCopy))
               M_VTBLID(class, onCopy) = METHOD(class, onCopy)->vid;

            METHOD(class, onDisplay) = Class_findMethod(CO(class), "OnDisplay", app);
            if(METHOD(class, onDisplay))
               M_VTBLID(class, onDisplay) = METHOD(class, onDisplay)->vid;

            METHOD(class, onEdit) = Class_findMethod(CO(class), "OnEdit", app);
            if(METHOD(class, onEdit))
               M_VTBLID(class, onEdit) = METHOD(class, onEdit)->vid;

            METHOD(class, onFree) = Class_findMethod(CO(class), "OnFree", app);
            if(METHOD(class, onFree))
               M_VTBLID(class, onFree) = METHOD(class, onFree)->vid;

            METHOD(class, onGetDataFromString) = Class_findMethod(CO(class), "OnGetDataFromString", app);
            if(METHOD(class, onGetDataFromString))
               M_VTBLID(class, onGetDataFromString) = METHOD(class, onGetDataFromString)->vid;

            METHOD(class, onGetString) = Class_findMethod(CO(class), "OnGetString", app);
            if(METHOD(class, onGetString))
               M_VTBLID(class, onGetString) = METHOD(class, onGetString)->vid;

            METHOD(class, onSaveEdit) = Class_findMethod(CO(class), "OnSaveEdit", app);
            if(METHOD(class, onSaveEdit))
               M_VTBLID(class, onSaveEdit) = METHOD(class, onSaveEdit)->vid;

            METHOD(class, onSerialize) = Class_findMethod(CO(class), "OnSerialize", app);
            if(METHOD(class, onSerialize))
               M_VTBLID(class, onSerialize) = METHOD(class, onSerialize)->vid;

            METHOD(class, onUnserialize) = Class_findMethod(CO(class), "OnUnserialize", app);
            if(METHOD(class, onUnserialize))
               M_VTBLID(class, onUnserialize) = METHOD(class, onUnserialize)->vid;
         }
         CO(double) = eC_findClass(app, "double");
         if(CO(double))
         {
            METHOD(double, inf) = Class_findMethod(CO(double), "inf", app);
            if(METHOD(double, inf))
               double_inf = (double (*)(void))METHOD(double, inf)->function;

            METHOD(double, nan) = Class_findMethod(CO(double), "nan", app);
            if(METHOD(double, nan))
               double_nan = (double (*)(void))METHOD(double, nan)->function;

            PROPERTY(double, isNan) = Class_findProperty(CO(double), "isNan", app);
            if(PROPERTY(double, isNan))
               double_get_isNan = (void *)PROPERTY(double, isNan)->Get;

            PROPERTY(double, isInf) = Class_findProperty(CO(double), "isInf", app);
            if(PROPERTY(double, isInf))
               double_get_isInf = (void *)PROPERTY(double, isInf)->Get;

            PROPERTY(double, signBit) = Class_findProperty(CO(double), "signBit", app);
            if(PROPERTY(double, signBit))
               double_get_signBit = (void *)PROPERTY(double, signBit)->Get;
         }
         CO(enum) = eC_findClass(app, "enum");
         CO(float) = eC_findClass(app, "float");
         if(CO(float))
         {
            METHOD(float, inf) = Class_findMethod(CO(float), "inf", app);
            if(METHOD(float, inf))
               float_inf = (float (*)(void))METHOD(float, inf)->function;

            METHOD(float, nan) = Class_findMethod(CO(float), "nan", app);
            if(METHOD(float, nan))
               float_nan = (float (*)(void))METHOD(float, nan)->function;

            PROPERTY(float, isNan) = Class_findProperty(CO(float), "isNan", app);
            if(PROPERTY(float, isNan))
               float_get_isNan = (void *)PROPERTY(float, isNan)->Get;

            PROPERTY(float, isInf) = Class_findProperty(CO(float), "isInf", app);
            if(PROPERTY(float, isInf))
               float_get_isInf = (void *)PROPERTY(float, isInf)->Get;

            PROPERTY(float, signBit) = Class_findProperty(CO(float), "signBit", app);
            if(PROPERTY(float, signBit))
               float_get_signBit = (void *)PROPERTY(float, signBit)->Get;
         }
         CO(int) = eC_findClass(app, "int");
         CO(int64) = eC_findClass(app, "int64");
         CO(intptr) = eC_findClass(app, "intptr");
         CO(intsize) = eC_findClass(app, "intsize");
         CO(short) = eC_findClass(app, "short");
         CO(struct) = eC_findClass(app, "struct");
         CO(uint) = eC_findClass(app, "uint");
         CO(uint16) = eC_findClass(app, "uint16");
         CO(uint32) = eC_findClass(app, "uint32");
         CO(uint64) = eC_findClass(app, "uint64");
         CO(uintptr) = eC_findClass(app, "uintptr");
         CO(uintsize) = eC_findClass(app, "uintsize");
         CO(Application) = eC_findClass(app, "Application");
         if(CO(Application))
         {
            METHOD(Application, main) = Class_findMethod(CO(Application), "Main", app);
            if(METHOD(Application, main))
               M_VTBLID(Application, main) = METHOD(Application, main)->vid;
         }
         CO(Instance) = eC_findClass(app, "Instance");
         CO(Module) = eC_findClass(app, "Module");
         if(CO(Module))
         {
            METHOD(Module, onLoad) = Class_findMethod(CO(Module), "OnLoad", app);
            if(METHOD(Module, onLoad))
               M_VTBLID(Module, onLoad) = METHOD(Module, onLoad)->vid;

            METHOD(Module, onUnload) = Class_findMethod(CO(Module), "OnUnload", app);
            if(METHOD(Module, onUnload))
               M_VTBLID(Module, onUnload) = METHOD(Module, onUnload)->vid;
         }
         CO(unichar) = eC_findClass(app, "unichar");
         CO(FieldType) = eC_findClass(app, "FieldType");
         CO(FieldTypeEx) = eC_findClass(app, "FieldTypeEx");
         CO(FieldValue) = eC_findClass(app, "FieldValue");
         if(CO(FieldValue))
         {
            METHOD(FieldValue, compareInt) = Class_findMethod(CO(FieldValue), "compareInt", app);
            if(METHOD(FieldValue, compareInt))
               FieldValue_compareInt = (int (*)(C(FieldValue) *, C(FieldValue) *))METHOD(FieldValue, compareInt)->function;

            METHOD(FieldValue, compareReal) = Class_findMethod(CO(FieldValue), "compareReal", app);
            if(METHOD(FieldValue, compareReal))
               FieldValue_compareReal = (int (*)(C(FieldValue) *, C(FieldValue) *))METHOD(FieldValue, compareReal)->function;

            METHOD(FieldValue, compareText) = Class_findMethod(CO(FieldValue), "compareText", app);
            if(METHOD(FieldValue, compareText))
               FieldValue_compareText = (int (*)(C(FieldValue) *, C(FieldValue) *))METHOD(FieldValue, compareText)->function;

            METHOD(FieldValue, formatArray) = Class_findMethod(CO(FieldValue), "formatArray", app);
            if(METHOD(FieldValue, formatArray))
               FieldValue_formatArray = (C(String) (*)(C(FieldValue) *, char *, void *, C(ObjectNotationType) *))METHOD(FieldValue, formatArray)->function;

            METHOD(FieldValue, formatFloat) = Class_findMethod(CO(FieldValue), "formatFloat", app);
            if(METHOD(FieldValue, formatFloat))
               FieldValue_formatFloat = (C(String) (*)(C(FieldValue) *, char *, C(bool)))METHOD(FieldValue, formatFloat)->function;

            METHOD(FieldValue, formatInteger) = Class_findMethod(CO(FieldValue), "formatInteger", app);
            if(METHOD(FieldValue, formatInteger))
               FieldValue_formatInteger = (C(String) (*)(C(FieldValue) *, char *))METHOD(FieldValue, formatInteger)->function;

            METHOD(FieldValue, formatMap) = Class_findMethod(CO(FieldValue), "formatMap", app);
            if(METHOD(FieldValue, formatMap))
               FieldValue_formatMap = (C(String) (*)(C(FieldValue) *, char *, void *, C(ObjectNotationType) *))METHOD(FieldValue, formatMap)->function;

            METHOD(FieldValue, getArrayOrMap) = Class_findMethod(CO(FieldValue), "getArrayOrMap", app);
            if(METHOD(FieldValue, getArrayOrMap))
               FieldValue_getArrayOrMap = (C(bool) (*)(const char *, C(Class) *, void **))METHOD(FieldValue, getArrayOrMap)->function;

            METHOD(FieldValue, stringify) = Class_findMethod(CO(FieldValue), "stringify", app);
            if(METHOD(FieldValue, stringify))
               FieldValue_stringify = (C(String) (*)(C(FieldValue) *))METHOD(FieldValue, stringify)->function;
         }
         CO(FieldValueFormat) = eC_findClass(app, "FieldValueFormat");
         CO(AVLNode) = eC_findClass(app, "AVLNode");
         if(CO(AVLNode))
         {
            METHOD(AVLNode, find) = Class_findMethod(CO(AVLNode), "Find", app);
            if(METHOD(AVLNode, find))
               AVLNode_find = (thisclass(AVLNode *) (*)(C(AVLNode) *, C(Class) *, TP(AVLNode, T)))METHOD(AVLNode, find)->function;

            PROPERTY(AVLNode, prev) = Class_findProperty(CO(AVLNode), "prev", app);
            if(PROPERTY(AVLNode, prev))
               AVLNode_get_prev = (void *)PROPERTY(AVLNode, prev)->Get;

            PROPERTY(AVLNode, next) = Class_findProperty(CO(AVLNode), "next", app);
            if(PROPERTY(AVLNode, next))
               AVLNode_get_next = (void *)PROPERTY(AVLNode, next)->Get;

            PROPERTY(AVLNode, minimum) = Class_findProperty(CO(AVLNode), "minimum", app);
            if(PROPERTY(AVLNode, minimum))
               AVLNode_get_minimum = (void *)PROPERTY(AVLNode, minimum)->Get;

            PROPERTY(AVLNode, maximum) = Class_findProperty(CO(AVLNode), "maximum", app);
            if(PROPERTY(AVLNode, maximum))
               AVLNode_get_maximum = (void *)PROPERTY(AVLNode, maximum)->Get;

            PROPERTY(AVLNode, count) = Class_findProperty(CO(AVLNode), "count", app);
            if(PROPERTY(AVLNode, count))
               AVLNode_get_count = (void *)PROPERTY(AVLNode, count)->Get;

            PROPERTY(AVLNode, depthProp) = Class_findProperty(CO(AVLNode), "depthProp", app);
            if(PROPERTY(AVLNode, depthProp))
               AVLNode_get_depthProp = (void *)PROPERTY(AVLNode, depthProp)->Get;
         }
         CO(AVLTree) = eC_findClass(app, "AVLTree");
         CO(Array) = eC_findClass(app, "Array");
         if(CO(Array))
         {
            PROPERTY(Array, size) = Class_findProperty(CO(Array), "size", app);
            if(PROPERTY(Array, size))
            {
               Array_set_size = (void *)PROPERTY(Array, size)->Set;
               Array_get_size = (void *)PROPERTY(Array, size)->Get;
            }

            PROPERTY(Array, minAllocSize) = Class_findProperty(CO(Array), "minAllocSize", app);
            if(PROPERTY(Array, minAllocSize))
            {
               Array_set_minAllocSize = (void *)PROPERTY(Array, minAllocSize)->Set;
               Array_get_minAllocSize = (void *)PROPERTY(Array, minAllocSize)->Get;
            }
         }
         CO(BTNode) = eC_findClass(app, "BTNode");
         if(CO(BTNode))
         {
            METHOD(BTNode, findPrefix) = Class_findMethod(CO(BTNode), "FindPrefix", app);
            if(METHOD(BTNode, findPrefix))
               BTNode_findPrefix = (C(BTNode) * (*)(C(BTNode) *, const char *))METHOD(BTNode, findPrefix)->function;

            METHOD(BTNode, findString) = Class_findMethod(CO(BTNode), "FindString", app);
            if(METHOD(BTNode, findString))
               BTNode_findString = (C(BTNode) * (*)(C(BTNode) *, const char *))METHOD(BTNode, findString)->function;

            PROPERTY(BTNode, prev) = Class_findProperty(CO(BTNode), "prev", app);
            if(PROPERTY(BTNode, prev))
               BTNode_get_prev = (void *)PROPERTY(BTNode, prev)->Get;

            PROPERTY(BTNode, next) = Class_findProperty(CO(BTNode), "next", app);
            if(PROPERTY(BTNode, next))
               BTNode_get_next = (void *)PROPERTY(BTNode, next)->Get;

            PROPERTY(BTNode, minimum) = Class_findProperty(CO(BTNode), "minimum", app);
            if(PROPERTY(BTNode, minimum))
               BTNode_get_minimum = (void *)PROPERTY(BTNode, minimum)->Get;

            PROPERTY(BTNode, maximum) = Class_findProperty(CO(BTNode), "maximum", app);
            if(PROPERTY(BTNode, maximum))
               BTNode_get_maximum = (void *)PROPERTY(BTNode, maximum)->Get;

            PROPERTY(BTNode, count) = Class_findProperty(CO(BTNode), "count", app);
            if(PROPERTY(BTNode, count))
               BTNode_get_count = (void *)PROPERTY(BTNode, count)->Get;

            PROPERTY(BTNode, depthProp) = Class_findProperty(CO(BTNode), "depthProp", app);
            if(PROPERTY(BTNode, depthProp))
               BTNode_get_depthProp = (void *)PROPERTY(BTNode, depthProp)->Get;
         }
         CO(BinaryTree) = eC_findClass(app, "BinaryTree");
         if(CO(BinaryTree))
         {
            METHOD(BinaryTree, add) = Class_findMethod(CO(BinaryTree), "Add", app);
            if(METHOD(BinaryTree, add))
               BinaryTree_add = (C(bool) (*)(C(BinaryTree) *, C(BTNode) *))METHOD(BinaryTree, add)->function;

            METHOD(BinaryTree, check) = Class_findMethod(CO(BinaryTree), "Check", app);
            if(METHOD(BinaryTree, check))
               BinaryTree_check = (C(bool) (*)(C(BinaryTree) *))METHOD(BinaryTree, check)->function;

            METHOD(BinaryTree, compareInt) = Class_findMethod(CO(BinaryTree), "CompareInt", app);
            if(METHOD(BinaryTree, compareInt))
               BinaryTree_compareInt = (int (*)(C(BinaryTree) *, uintptr, uintptr))METHOD(BinaryTree, compareInt)->function;

            METHOD(BinaryTree, compareString) = Class_findMethod(CO(BinaryTree), "CompareString", app);
            if(METHOD(BinaryTree, compareString))
               BinaryTree_compareString = (int (*)(C(BinaryTree) *, const char *, const char *))METHOD(BinaryTree, compareString)->function;

            METHOD(BinaryTree, delete) = Class_findMethod(CO(BinaryTree), "Delete", app);
            if(METHOD(BinaryTree, delete))
               BinaryTree_delete = (void (*)(C(BinaryTree) *, C(BTNode) *))METHOD(BinaryTree, delete)->function;

            METHOD(BinaryTree, find) = Class_findMethod(CO(BinaryTree), "Find", app);
            if(METHOD(BinaryTree, find))
               BinaryTree_find = (C(BTNode) * (*)(C(BinaryTree) *, uintptr))METHOD(BinaryTree, find)->function;

            METHOD(BinaryTree, findAll) = Class_findMethod(CO(BinaryTree), "FindAll", app);
            if(METHOD(BinaryTree, findAll))
               BinaryTree_findAll = (C(BTNode) * (*)(C(BinaryTree) *, uintptr))METHOD(BinaryTree, findAll)->function;

            METHOD(BinaryTree, findPrefix) = Class_findMethod(CO(BinaryTree), "FindPrefix", app);
            if(METHOD(BinaryTree, findPrefix))
               BinaryTree_findPrefix = (C(BTNode) * (*)(C(BinaryTree) *, const char *))METHOD(BinaryTree, findPrefix)->function;

            METHOD(BinaryTree, findString) = Class_findMethod(CO(BinaryTree), "FindString", app);
            if(METHOD(BinaryTree, findString))
               BinaryTree_findString = (C(BTNode) * (*)(C(BinaryTree) *, const char *))METHOD(BinaryTree, findString)->function;

            METHOD(BinaryTree, free) = Class_findMethod(CO(BinaryTree), "Free", app);
            if(METHOD(BinaryTree, free))
               BinaryTree_free = (void (*)(C(BinaryTree) *))METHOD(BinaryTree, free)->function;

            METHOD(BinaryTree, freeString) = Class_findMethod(CO(BinaryTree), "FreeString", app);
            if(METHOD(BinaryTree, freeString))
               BinaryTree_freeString = (void (*)(char *))METHOD(BinaryTree, freeString)->function;

            METHOD(BinaryTree, print) = Class_findMethod(CO(BinaryTree), "Print", app);
            if(METHOD(BinaryTree, print))
               BinaryTree_print = (char * (*)(C(BinaryTree) *, char *, C(TreePrintStyle)))METHOD(BinaryTree, print)->function;

            METHOD(BinaryTree, remove) = Class_findMethod(CO(BinaryTree), "Remove", app);
            if(METHOD(BinaryTree, remove))
               BinaryTree_remove = (void (*)(C(BinaryTree) *, C(BTNode) *))METHOD(BinaryTree, remove)->function;

            PROPERTY(BinaryTree, first) = Class_findProperty(CO(BinaryTree), "first", app);
            if(PROPERTY(BinaryTree, first))
               BinaryTree_get_first = (void *)PROPERTY(BinaryTree, first)->Get;

            PROPERTY(BinaryTree, last) = Class_findProperty(CO(BinaryTree), "last", app);
            if(PROPERTY(BinaryTree, last))
               BinaryTree_get_last = (void *)PROPERTY(BinaryTree, last)->Get;
         }
         CO(BuiltInContainer) = eC_findClass(app, "BuiltInContainer");
         if(CO(BuiltInContainer))
         {
            METHOD(BuiltInContainer, add) = Class_findMethod(CO(BuiltInContainer), "Add", app);
            if(METHOD(BuiltInContainer, add))
               M_VTBLID(BuiltInContainer, add) = METHOD(BuiltInContainer, add)->vid;

            METHOD(BuiltInContainer, copy) = Class_findMethod(CO(BuiltInContainer), "Copy", app);
            if(METHOD(BuiltInContainer, copy))
               M_VTBLID(BuiltInContainer, copy) = METHOD(BuiltInContainer, copy)->vid;

            METHOD(BuiltInContainer, delete) = Class_findMethod(CO(BuiltInContainer), "Delete", app);
            if(METHOD(BuiltInContainer, delete))
               M_VTBLID(BuiltInContainer, delete) = METHOD(BuiltInContainer, delete)->vid;

            METHOD(BuiltInContainer, find) = Class_findMethod(CO(BuiltInContainer), "Find", app);
            if(METHOD(BuiltInContainer, find))
               M_VTBLID(BuiltInContainer, find) = METHOD(BuiltInContainer, find)->vid;

            METHOD(BuiltInContainer, free) = Class_findMethod(CO(BuiltInContainer), "Free", app);
            if(METHOD(BuiltInContainer, free))
               M_VTBLID(BuiltInContainer, free) = METHOD(BuiltInContainer, free)->vid;

            METHOD(BuiltInContainer, freeIterator) = Class_findMethod(CO(BuiltInContainer), "FreeIterator", app);
            if(METHOD(BuiltInContainer, freeIterator))
               M_VTBLID(BuiltInContainer, freeIterator) = METHOD(BuiltInContainer, freeIterator)->vid;

            METHOD(BuiltInContainer, getAtPosition) = Class_findMethod(CO(BuiltInContainer), "GetAtPosition", app);
            if(METHOD(BuiltInContainer, getAtPosition))
               M_VTBLID(BuiltInContainer, getAtPosition) = METHOD(BuiltInContainer, getAtPosition)->vid;

            METHOD(BuiltInContainer, getCount) = Class_findMethod(CO(BuiltInContainer), "GetCount", app);
            if(METHOD(BuiltInContainer, getCount))
               M_VTBLID(BuiltInContainer, getCount) = METHOD(BuiltInContainer, getCount)->vid;

            METHOD(BuiltInContainer, getData) = Class_findMethod(CO(BuiltInContainer), "GetData", app);
            if(METHOD(BuiltInContainer, getData))
               M_VTBLID(BuiltInContainer, getData) = METHOD(BuiltInContainer, getData)->vid;

            METHOD(BuiltInContainer, getFirst) = Class_findMethod(CO(BuiltInContainer), "GetFirst", app);
            if(METHOD(BuiltInContainer, getFirst))
               M_VTBLID(BuiltInContainer, getFirst) = METHOD(BuiltInContainer, getFirst)->vid;

            METHOD(BuiltInContainer, getLast) = Class_findMethod(CO(BuiltInContainer), "GetLast", app);
            if(METHOD(BuiltInContainer, getLast))
               M_VTBLID(BuiltInContainer, getLast) = METHOD(BuiltInContainer, getLast)->vid;

            METHOD(BuiltInContainer, getNext) = Class_findMethod(CO(BuiltInContainer), "GetNext", app);
            if(METHOD(BuiltInContainer, getNext))
               M_VTBLID(BuiltInContainer, getNext) = METHOD(BuiltInContainer, getNext)->vid;

            METHOD(BuiltInContainer, getPrev) = Class_findMethod(CO(BuiltInContainer), "GetPrev", app);
            if(METHOD(BuiltInContainer, getPrev))
               M_VTBLID(BuiltInContainer, getPrev) = METHOD(BuiltInContainer, getPrev)->vid;

            METHOD(BuiltInContainer, insert) = Class_findMethod(CO(BuiltInContainer), "Insert", app);
            if(METHOD(BuiltInContainer, insert))
               M_VTBLID(BuiltInContainer, insert) = METHOD(BuiltInContainer, insert)->vid;

            METHOD(BuiltInContainer, move) = Class_findMethod(CO(BuiltInContainer), "Move", app);
            if(METHOD(BuiltInContainer, move))
               M_VTBLID(BuiltInContainer, move) = METHOD(BuiltInContainer, move)->vid;

            METHOD(BuiltInContainer, remove) = Class_findMethod(CO(BuiltInContainer), "Remove", app);
            if(METHOD(BuiltInContainer, remove))
               M_VTBLID(BuiltInContainer, remove) = METHOD(BuiltInContainer, remove)->vid;

            METHOD(BuiltInContainer, removeAll) = Class_findMethod(CO(BuiltInContainer), "RemoveAll", app);
            if(METHOD(BuiltInContainer, removeAll))
               M_VTBLID(BuiltInContainer, removeAll) = METHOD(BuiltInContainer, removeAll)->vid;

            METHOD(BuiltInContainer, setData) = Class_findMethod(CO(BuiltInContainer), "SetData", app);
            if(METHOD(BuiltInContainer, setData))
               M_VTBLID(BuiltInContainer, setData) = METHOD(BuiltInContainer, setData)->vid;

            METHOD(BuiltInContainer, sort) = Class_findMethod(CO(BuiltInContainer), "Sort", app);
            if(METHOD(BuiltInContainer, sort))
               M_VTBLID(BuiltInContainer, sort) = METHOD(BuiltInContainer, sort)->vid;

            PROPERTY(BuiltInContainer, Container) = Class_findProperty(CO(BuiltInContainer), "eC::containers::Container", app);
            if(PROPERTY(BuiltInContainer, Container))
               BuiltInContainer_to_Container = (void *)PROPERTY(BuiltInContainer, Container)->Get;
         }
         CO(Container) = eC_findClass(app, "Container");
         if(CO(Container))
         {
            METHOD(Container, add) = Class_findMethod(CO(Container), "Add", app);
            if(METHOD(Container, add))
               M_VTBLID(Container, add) = METHOD(Container, add)->vid;

            METHOD(Container, copy) = Class_findMethod(CO(Container), "Copy", app);
            if(METHOD(Container, copy))
               M_VTBLID(Container, copy) = METHOD(Container, copy)->vid;

            METHOD(Container, delete) = Class_findMethod(CO(Container), "Delete", app);
            if(METHOD(Container, delete))
               M_VTBLID(Container, delete) = METHOD(Container, delete)->vid;

            METHOD(Container, find) = Class_findMethod(CO(Container), "Find", app);
            if(METHOD(Container, find))
               M_VTBLID(Container, find) = METHOD(Container, find)->vid;

            METHOD(Container, free) = Class_findMethod(CO(Container), "Free", app);
            if(METHOD(Container, free))
               M_VTBLID(Container, free) = METHOD(Container, free)->vid;

            METHOD(Container, freeIterator) = Class_findMethod(CO(Container), "FreeIterator", app);
            if(METHOD(Container, freeIterator))
               M_VTBLID(Container, freeIterator) = METHOD(Container, freeIterator)->vid;

            METHOD(Container, getAtPosition) = Class_findMethod(CO(Container), "GetAtPosition", app);
            if(METHOD(Container, getAtPosition))
               M_VTBLID(Container, getAtPosition) = METHOD(Container, getAtPosition)->vid;

            METHOD(Container, getCount) = Class_findMethod(CO(Container), "GetCount", app);
            if(METHOD(Container, getCount))
               M_VTBLID(Container, getCount) = METHOD(Container, getCount)->vid;

            METHOD(Container, getData) = Class_findMethod(CO(Container), "GetData", app);
            if(METHOD(Container, getData))
               M_VTBLID(Container, getData) = METHOD(Container, getData)->vid;

            METHOD(Container, getFirst) = Class_findMethod(CO(Container), "GetFirst", app);
            if(METHOD(Container, getFirst))
               M_VTBLID(Container, getFirst) = METHOD(Container, getFirst)->vid;

            METHOD(Container, getLast) = Class_findMethod(CO(Container), "GetLast", app);
            if(METHOD(Container, getLast))
               M_VTBLID(Container, getLast) = METHOD(Container, getLast)->vid;

            METHOD(Container, getNext) = Class_findMethod(CO(Container), "GetNext", app);
            if(METHOD(Container, getNext))
               M_VTBLID(Container, getNext) = METHOD(Container, getNext)->vid;

            METHOD(Container, getPrev) = Class_findMethod(CO(Container), "GetPrev", app);
            if(METHOD(Container, getPrev))
               M_VTBLID(Container, getPrev) = METHOD(Container, getPrev)->vid;

            METHOD(Container, insert) = Class_findMethod(CO(Container), "Insert", app);
            if(METHOD(Container, insert))
               M_VTBLID(Container, insert) = METHOD(Container, insert)->vid;

            METHOD(Container, move) = Class_findMethod(CO(Container), "Move", app);
            if(METHOD(Container, move))
               M_VTBLID(Container, move) = METHOD(Container, move)->vid;

            METHOD(Container, remove) = Class_findMethod(CO(Container), "Remove", app);
            if(METHOD(Container, remove))
               M_VTBLID(Container, remove) = METHOD(Container, remove)->vid;

            METHOD(Container, removeAll) = Class_findMethod(CO(Container), "RemoveAll", app);
            if(METHOD(Container, removeAll))
               M_VTBLID(Container, removeAll) = METHOD(Container, removeAll)->vid;

            METHOD(Container, setData) = Class_findMethod(CO(Container), "SetData", app);
            if(METHOD(Container, setData))
               M_VTBLID(Container, setData) = METHOD(Container, setData)->vid;

            METHOD(Container, sort) = Class_findMethod(CO(Container), "Sort", app);
            if(METHOD(Container, sort))
               M_VTBLID(Container, sort) = METHOD(Container, sort)->vid;

            METHOD(Container, takeOut) = Class_findMethod(CO(Container), "TakeOut", app);
            if(METHOD(Container, takeOut))
               Container_takeOut = (C(bool) (*)(C(Container), TP(Container, D)))METHOD(Container, takeOut)->function;

            PROPERTY(Container, copySrc) = Class_findProperty(CO(Container), "copySrc", app);
            if(PROPERTY(Container, copySrc))
               Container_set_copySrc = (void *)PROPERTY(Container, copySrc)->Set;

            PROPERTY(Container, firstIterator) = Class_findProperty(CO(Container), "firstIterator", app);
            if(PROPERTY(Container, firstIterator))
               Container_get_firstIterator = (void *)PROPERTY(Container, firstIterator)->Get;

            PROPERTY(Container, lastIterator) = Class_findProperty(CO(Container), "lastIterator", app);
            if(PROPERTY(Container, lastIterator))
               Container_get_lastIterator = (void *)PROPERTY(Container, lastIterator)->Get;
         }
         CO(CustomAVLTree) = eC_findClass(app, "CustomAVLTree");
         if(CO(CustomAVLTree))
         {
            METHOD(CustomAVLTree, check) = Class_findMethod(CO(CustomAVLTree), "Check", app);
            if(METHOD(CustomAVLTree, check))
               CustomAVLTree_check = (C(bool) (*)(C(CustomAVLTree)))METHOD(CustomAVLTree, check)->function;

            METHOD(CustomAVLTree, freeKey) = Class_findMethod(CO(CustomAVLTree), "FreeKey", app);
            if(METHOD(CustomAVLTree, freeKey))
               CustomAVLTree_freeKey = (void (*)(C(CustomAVLTree), C(AVLNode) *))METHOD(CustomAVLTree, freeKey)->function;
         }
         CO(HashMap) = eC_findClass(app, "HashMap");
         if(CO(HashMap))
         {
            METHOD(HashMap, removeIterating) = Class_findMethod(CO(HashMap), "removeIterating", app);
            if(METHOD(HashMap, removeIterating))
               HashMap_removeIterating = (void (*)(C(HashMap), C(IteratorPointer) *))METHOD(HashMap, removeIterating)->function;

            METHOD(HashMap, resize) = Class_findMethod(CO(HashMap), "resize", app);
            if(METHOD(HashMap, resize))
               HashMap_resize = (void (*)(C(HashMap), C(IteratorPointer) *))METHOD(HashMap, resize)->function;

            PROPERTY(HashMap, count) = Class_findProperty(CO(HashMap), "count", app);
            if(PROPERTY(HashMap, count))
               HashMap_get_count = (void *)PROPERTY(HashMap, count)->Get;

            PROPERTY(HashMap, initSize) = Class_findProperty(CO(HashMap), "initSize", app);
            if(PROPERTY(HashMap, initSize))
               HashMap_set_initSize = (void *)PROPERTY(HashMap, initSize)->Set;
         }
         CO(HashMapIterator) = eC_findClass(app, "HashMapIterator");
         if(CO(HashMapIterator))
         {
            PROPERTY(HashMapIterator, map) = Class_findProperty(CO(HashMapIterator), "map", app);
            if(PROPERTY(HashMapIterator, map))
            {
               HashMapIterator_set_map = (void *)PROPERTY(HashMapIterator, map)->Set;
               HashMapIterator_get_map = (void *)PROPERTY(HashMapIterator, map)->Get;
            }

            PROPERTY(HashMapIterator, key) = Class_findProperty(CO(HashMapIterator), "key", app);
            if(PROPERTY(HashMapIterator, key))
               HashMapIterator_get_key = (void *)PROPERTY(HashMapIterator, key)->Get;

            PROPERTY(HashMapIterator, value) = Class_findProperty(CO(HashMapIterator), "value", app);
            if(PROPERTY(HashMapIterator, value))
            {
               HashMapIterator_set_value = (void *)PROPERTY(HashMapIterator, value)->Set;
               HashMapIterator_get_value = (void *)PROPERTY(HashMapIterator, value)->Get;
            }
         }
         CO(HashTable) = eC_findClass(app, "HashTable");
         if(CO(HashTable))
         {
            PROPERTY(HashTable, initSize) = Class_findProperty(CO(HashTable), "initSize", app);
            if(PROPERTY(HashTable, initSize))
               HashTable_set_initSize = (void *)PROPERTY(HashTable, initSize)->Set;
         }
         CO(Item) = eC_findClass(app, "Item");
         if(CO(Item))
         {
            METHOD(Item, copy) = Class_findMethod(CO(Item), "Copy", app);
            if(METHOD(Item, copy))
               Item_copy = (void (*)(C(Item) *, C(Item) *, int))METHOD(Item, copy)->function;
         }
         CO(Iterator) = eC_findClass(app, "Iterator");
         if(CO(Iterator))
         {
            METHOD(Iterator, find) = Class_findMethod(CO(Iterator), "Find", app);
            if(METHOD(Iterator, find))
               Iterator_find = (C(bool) (*)(C(Iterator) *, TP(Iterator, T)))METHOD(Iterator, find)->function;

            METHOD(Iterator, free) = Class_findMethod(CO(Iterator), "Free", app);
            if(METHOD(Iterator, free))
               Iterator_free = (void (*)(C(Iterator) *))METHOD(Iterator, free)->function;

            METHOD(Iterator, getData) = Class_findMethod(CO(Iterator), "GetData", app);
            if(METHOD(Iterator, getData))
               Iterator_getData = (TP(Iterator, T) (*)(C(Iterator) *))METHOD(Iterator, getData)->function;

            METHOD(Iterator, index) = Class_findMethod(CO(Iterator), "Index", app);
            if(METHOD(Iterator, index))
               Iterator_index = (C(bool) (*)(C(Iterator) *, TP(Iterator, IT), C(bool)))METHOD(Iterator, index)->function;

            METHOD(Iterator, next) = Class_findMethod(CO(Iterator), "Next", app);
            if(METHOD(Iterator, next))
               Iterator_next = (C(bool) (*)(C(Iterator) *))METHOD(Iterator, next)->function;

            METHOD(Iterator, prev) = Class_findMethod(CO(Iterator), "Prev", app);
            if(METHOD(Iterator, prev))
               Iterator_prev = (C(bool) (*)(C(Iterator) *))METHOD(Iterator, prev)->function;

            METHOD(Iterator, remove) = Class_findMethod(CO(Iterator), "Remove", app);
            if(METHOD(Iterator, remove))
               Iterator_remove = (void (*)(C(Iterator) *))METHOD(Iterator, remove)->function;

            METHOD(Iterator, setData) = Class_findMethod(CO(Iterator), "SetData", app);
            if(METHOD(Iterator, setData))
               Iterator_setData = (C(bool) (*)(C(Iterator) *, TP(Iterator, T)))METHOD(Iterator, setData)->function;

            PROPERTY(Iterator, data) = Class_findProperty(CO(Iterator), "data", app);
            if(PROPERTY(Iterator, data))
            {
               Iterator_set_data = (void *)PROPERTY(Iterator, data)->Set;
               Iterator_get_data = (void *)PROPERTY(Iterator, data)->Get;
            }
         }
         CO(IteratorPointer) = eC_findClass(app, "IteratorPointer");
         CO(Link) = eC_findClass(app, "Link");
         CO(LinkElement) = eC_findClass(app, "LinkElement");
         CO(LinkList) = eC_findClass(app, "LinkList");
         if(CO(LinkList))
         {
            METHOD(LinkList, _Sort) = Class_findMethod(CO(LinkList), "_Sort", app);
            if(METHOD(LinkList, _Sort))
               LinkList__Sort = (void (*)(C(LinkList), C(bool), C(LinkList) *))METHOD(LinkList, _Sort)->function;
         }
         CO(List) = eC_findClass(app, "List");
         CO(ListItem) = eC_findClass(app, "ListItem");
         CO(Map) = eC_findClass(app, "Map");
         if(CO(Map))
         {
            PROPERTY(Map, mapSrc) = Class_findProperty(CO(Map), "mapSrc", app);
            if(PROPERTY(Map, mapSrc))
               Map_set_mapSrc = (void *)PROPERTY(Map, mapSrc)->Set;
         }
         CO(MapIterator) = eC_findClass(app, "MapIterator");
         if(CO(MapIterator))
         {
            PROPERTY(MapIterator, map) = Class_findProperty(CO(MapIterator), "map", app);
            if(PROPERTY(MapIterator, map))
            {
               MapIterator_set_map = (void *)PROPERTY(MapIterator, map)->Set;
               MapIterator_get_map = (void *)PROPERTY(MapIterator, map)->Get;
            }

            PROPERTY(MapIterator, key) = Class_findProperty(CO(MapIterator), "key", app);
            if(PROPERTY(MapIterator, key))
               MapIterator_get_key = (void *)PROPERTY(MapIterator, key)->Get;

            PROPERTY(MapIterator, value) = Class_findProperty(CO(MapIterator), "value", app);
            if(PROPERTY(MapIterator, value))
            {
               MapIterator_set_value = (void *)PROPERTY(MapIterator, value)->Set;
               MapIterator_get_value = (void *)PROPERTY(MapIterator, value)->Get;
            }
         }
         CO(MapNode) = eC_findClass(app, "MapNode");
         if(CO(MapNode))
         {
            PROPERTY(MapNode, key) = Class_findProperty(CO(MapNode), "key", app);
            if(PROPERTY(MapNode, key))
            {
               MapNode_set_key = (void *)PROPERTY(MapNode, key)->Set;
               MapNode_get_key = (void *)PROPERTY(MapNode, key)->Get;
            }

            PROPERTY(MapNode, value) = Class_findProperty(CO(MapNode), "value", app);
            if(PROPERTY(MapNode, value))
            {
               MapNode_set_value = (void *)PROPERTY(MapNode, value)->Set;
               MapNode_get_value = (void *)PROPERTY(MapNode, value)->Get;
            }

            PROPERTY(MapNode, prev) = Class_findProperty(CO(MapNode), "prev", app);
            if(PROPERTY(MapNode, prev))
               MapNode_get_prev = (void *)PROPERTY(MapNode, prev)->Get;

            PROPERTY(MapNode, next) = Class_findProperty(CO(MapNode), "next", app);
            if(PROPERTY(MapNode, next))
               MapNode_get_next = (void *)PROPERTY(MapNode, next)->Get;

            PROPERTY(MapNode, minimum) = Class_findProperty(CO(MapNode), "minimum", app);
            if(PROPERTY(MapNode, minimum))
               MapNode_get_minimum = (void *)PROPERTY(MapNode, minimum)->Get;

            PROPERTY(MapNode, maximum) = Class_findProperty(CO(MapNode), "maximum", app);
            if(PROPERTY(MapNode, maximum))
               MapNode_get_maximum = (void *)PROPERTY(MapNode, maximum)->Get;
         }
         CO(NamedItem) = eC_findClass(app, "NamedItem");
         CO(NamedLink) = eC_findClass(app, "NamedLink");
         CO(NamedLink64) = eC_findClass(app, "NamedLink64");
         CO(OldLink) = eC_findClass(app, "OldLink");
         if(CO(OldLink))
         {
            METHOD(OldLink, free) = Class_findMethod(CO(OldLink), "Free", app);
            if(METHOD(OldLink, free))
               OldLink_free = (void (*)(C(OldLink) *))METHOD(OldLink, free)->function;
         }
         CO(OldList) = eC_findClass(app, "OldList");
         if(CO(OldList))
         {
            METHOD(OldList, add) = Class_findMethod(CO(OldList), "Add", app);
            if(METHOD(OldList, add))
               OldList_add = (void (*)(C(OldList) *, void *))METHOD(OldList, add)->function;

            METHOD(OldList, addName) = Class_findMethod(CO(OldList), "AddName", app);
            if(METHOD(OldList, addName))
               OldList_addName = (C(bool) (*)(C(OldList) *, void *))METHOD(OldList, addName)->function;

            METHOD(OldList, clear) = Class_findMethod(CO(OldList), "Clear", app);
            if(METHOD(OldList, clear))
               OldList_clear = (void (*)(C(OldList) *))METHOD(OldList, clear)->function;

            METHOD(OldList, copy) = Class_findMethod(CO(OldList), "Copy", app);
            if(METHOD(OldList, copy))
               OldList_copy = (void (*)(C(OldList) *, C(OldList) *, int, void (*)(void *, void *)))METHOD(OldList, copy)->function;

            METHOD(OldList, delete) = Class_findMethod(CO(OldList), "Delete", app);
            if(METHOD(OldList, delete))
               OldList_delete = (void (*)(C(OldList) *, void *))METHOD(OldList, delete)->function;

            METHOD(OldList, findLink) = Class_findMethod(CO(OldList), "FindLink", app);
            if(METHOD(OldList, findLink))
               OldList_findLink = (C(OldLink) * (*)(C(OldList) *, void *))METHOD(OldList, findLink)->function;

            METHOD(OldList, findName) = Class_findMethod(CO(OldList), "FindName", app);
            if(METHOD(OldList, findName))
               OldList_findName = (void * (*)(C(OldList) *, const char *, C(bool)))METHOD(OldList, findName)->function;

            METHOD(OldList, findNamedLink) = Class_findMethod(CO(OldList), "FindNamedLink", app);
            if(METHOD(OldList, findNamedLink))
               OldList_findNamedLink = (void * (*)(C(OldList) *, const char *, C(bool)))METHOD(OldList, findNamedLink)->function;

            METHOD(OldList, free) = Class_findMethod(CO(OldList), "Free", app);
            if(METHOD(OldList, free))
               OldList_free = (void (*)(C(OldList) *, void (*)(void *)))METHOD(OldList, free)->function;

            METHOD(OldList, insert) = Class_findMethod(CO(OldList), "Insert", app);
            if(METHOD(OldList, insert))
               OldList_insert = (C(bool) (*)(C(OldList) *, void *, void *))METHOD(OldList, insert)->function;

            METHOD(OldList, move) = Class_findMethod(CO(OldList), "Move", app);
            if(METHOD(OldList, move))
               OldList_move = (void (*)(C(OldList) *, void *, void *))METHOD(OldList, move)->function;

            METHOD(OldList, placeName) = Class_findMethod(CO(OldList), "PlaceName", app);
            if(METHOD(OldList, placeName))
               OldList_placeName = (C(bool) (*)(C(OldList) *, const char *, void **))METHOD(OldList, placeName)->function;

            METHOD(OldList, remove) = Class_findMethod(CO(OldList), "Remove", app);
            if(METHOD(OldList, remove))
               OldList_remove = (void (*)(C(OldList) *, void *))METHOD(OldList, remove)->function;

            METHOD(OldList, removeAll) = Class_findMethod(CO(OldList), "RemoveAll", app);
            if(METHOD(OldList, removeAll))
               OldList_removeAll = (void (*)(C(OldList) *, void (*)(void *)))METHOD(OldList, removeAll)->function;

            METHOD(OldList, sort) = Class_findMethod(CO(OldList), "Sort", app);
            if(METHOD(OldList, sort))
               OldList_sort = (void (*)(C(OldList) *, int (*)(void *, void *, void *), void *))METHOD(OldList, sort)->function;

            METHOD(OldList, swap) = Class_findMethod(CO(OldList), "Swap", app);
            if(METHOD(OldList, swap))
               OldList_swap = (void (*)(C(OldList) *, void *, void *))METHOD(OldList, swap)->function;
         }
         CO(StringBTNode) = eC_findClass(app, "StringBTNode");
         CO(StringBinaryTree) = eC_findClass(app, "StringBinaryTree");
         CO(TreePrintStyle) = eC_findClass(app, "TreePrintStyle");
         CO(Archive) = eC_findClass(app, "Archive");
         if(CO(Archive))
         {
            METHOD(Archive, clear) = Class_findMethod(CO(Archive), "Clear", app);
            if(METHOD(Archive, clear))
               M_VTBLID(Archive, clear) = METHOD(Archive, clear)->vid;

            METHOD(Archive, fileExists) = Class_findMethod(CO(Archive), "FileExists", app);
            if(METHOD(Archive, fileExists))
               M_VTBLID(Archive, fileExists) = METHOD(Archive, fileExists)->vid;

            METHOD(Archive, fileOpen) = Class_findMethod(CO(Archive), "FileOpen", app);
            if(METHOD(Archive, fileOpen))
               M_VTBLID(Archive, fileOpen) = METHOD(Archive, fileOpen)->vid;

            METHOD(Archive, fileOpenAtPosition) = Class_findMethod(CO(Archive), "FileOpenAtPosition", app);
            if(METHOD(Archive, fileOpenAtPosition))
               M_VTBLID(Archive, fileOpenAtPosition) = METHOD(Archive, fileOpenAtPosition)->vid;

            METHOD(Archive, fileOpenCompressed) = Class_findMethod(CO(Archive), "FileOpenCompressed", app);
            if(METHOD(Archive, fileOpenCompressed))
               M_VTBLID(Archive, fileOpenCompressed) = METHOD(Archive, fileOpenCompressed)->vid;

            METHOD(Archive, openDirectory) = Class_findMethod(CO(Archive), "OpenDirectory", app);
            if(METHOD(Archive, openDirectory))
               M_VTBLID(Archive, openDirectory) = METHOD(Archive, openDirectory)->vid;

            METHOD(Archive, setBufferRead) = Class_findMethod(CO(Archive), "SetBufferRead", app);
            if(METHOD(Archive, setBufferRead))
               M_VTBLID(Archive, setBufferRead) = METHOD(Archive, setBufferRead)->vid;

            METHOD(Archive, setBufferSize) = Class_findMethod(CO(Archive), "SetBufferSize", app);
            if(METHOD(Archive, setBufferSize))
               M_VTBLID(Archive, setBufferSize) = METHOD(Archive, setBufferSize)->vid;

            PROPERTY(Archive, totalSize) = Class_findProperty(CO(Archive), "totalSize", app);
            if(PROPERTY(Archive, totalSize))
            {
               Archive_set_totalSize = (void *)PROPERTY(Archive, totalSize)->Set;
               Archive_get_totalSize = (void *)PROPERTY(Archive, totalSize)->Get;
            }

            PROPERTY(Archive, bufferSize) = Class_findProperty(CO(Archive), "bufferSize", app);
            if(PROPERTY(Archive, bufferSize))
               Archive_set_bufferSize = (void *)PROPERTY(Archive, bufferSize)->Set;

            PROPERTY(Archive, bufferRead) = Class_findProperty(CO(Archive), "bufferRead", app);
            if(PROPERTY(Archive, bufferRead))
               Archive_set_bufferRead = (void *)PROPERTY(Archive, bufferRead)->Set;
         }
         CO(ArchiveAddMode) = eC_findClass(app, "ArchiveAddMode");
         CO(ArchiveDir) = eC_findClass(app, "ArchiveDir");
         if(CO(ArchiveDir))
         {
            METHOD(ArchiveDir, add) = Class_findMethod(CO(ArchiveDir), "Add", app);
            if(METHOD(ArchiveDir, add))
               ArchiveDir_add = (C(bool) (*)(C(ArchiveDir), const char *, const char *, C(ArchiveAddMode), int, int *, uint *))METHOD(ArchiveDir, add)->function;

            METHOD(ArchiveDir, addFromFile) = Class_findMethod(CO(ArchiveDir), "AddFromFile", app);
            if(METHOD(ArchiveDir, addFromFile))
               M_VTBLID(ArchiveDir, addFromFile) = METHOD(ArchiveDir, addFromFile)->vid;

            METHOD(ArchiveDir, addFromFileAtPosition) = Class_findMethod(CO(ArchiveDir), "AddFromFileAtPosition", app);
            if(METHOD(ArchiveDir, addFromFileAtPosition))
               M_VTBLID(ArchiveDir, addFromFileAtPosition) = METHOD(ArchiveDir, addFromFileAtPosition)->vid;

            METHOD(ArchiveDir, delete) = Class_findMethod(CO(ArchiveDir), "Delete", app);
            if(METHOD(ArchiveDir, delete))
               M_VTBLID(ArchiveDir, delete) = METHOD(ArchiveDir, delete)->vid;

            METHOD(ArchiveDir, fileExists) = Class_findMethod(CO(ArchiveDir), "FileExists", app);
            if(METHOD(ArchiveDir, fileExists))
               M_VTBLID(ArchiveDir, fileExists) = METHOD(ArchiveDir, fileExists)->vid;

            METHOD(ArchiveDir, fileOpen) = Class_findMethod(CO(ArchiveDir), "FileOpen", app);
            if(METHOD(ArchiveDir, fileOpen))
               M_VTBLID(ArchiveDir, fileOpen) = METHOD(ArchiveDir, fileOpen)->vid;

            METHOD(ArchiveDir, move) = Class_findMethod(CO(ArchiveDir), "Move", app);
            if(METHOD(ArchiveDir, move))
               M_VTBLID(ArchiveDir, move) = METHOD(ArchiveDir, move)->vid;

            METHOD(ArchiveDir, openDirectory) = Class_findMethod(CO(ArchiveDir), "OpenDirectory", app);
            if(METHOD(ArchiveDir, openDirectory))
               M_VTBLID(ArchiveDir, openDirectory) = METHOD(ArchiveDir, openDirectory)->vid;

            METHOD(ArchiveDir, rename) = Class_findMethod(CO(ArchiveDir), "Rename", app);
            if(METHOD(ArchiveDir, rename))
               M_VTBLID(ArchiveDir, rename) = METHOD(ArchiveDir, rename)->vid;
         }
         CO(ArchiveOpenFlags) = eC_findClass(app, "ArchiveOpenFlags");
         CO(BufferedFile) = eC_findClass(app, "BufferedFile");
         if(CO(BufferedFile))
         {
            PROPERTY(BufferedFile, handle) = Class_findProperty(CO(BufferedFile), "handle", app);
            if(PROPERTY(BufferedFile, handle))
            {
               BufferedFile_set_handle = (void *)PROPERTY(BufferedFile, handle)->Set;
               BufferedFile_get_handle = (void *)PROPERTY(BufferedFile, handle)->Get;
            }

            PROPERTY(BufferedFile, bufferSize) = Class_findProperty(CO(BufferedFile), "bufferSize", app);
            if(PROPERTY(BufferedFile, bufferSize))
            {
               BufferedFile_set_bufferSize = (void *)PROPERTY(BufferedFile, bufferSize)->Set;
               BufferedFile_get_bufferSize = (void *)PROPERTY(BufferedFile, bufferSize)->Get;
            }

            PROPERTY(BufferedFile, bufferRead) = Class_findProperty(CO(BufferedFile), "bufferRead", app);
            if(PROPERTY(BufferedFile, bufferRead))
            {
               BufferedFile_set_bufferRead = (void *)PROPERTY(BufferedFile, bufferRead)->Set;
               BufferedFile_get_bufferRead = (void *)PROPERTY(BufferedFile, bufferRead)->Get;
            }
         }
         CO(ConsoleFile) = eC_findClass(app, "ConsoleFile");
         CO(DualPipe) = eC_findClass(app, "DualPipe");
         if(CO(DualPipe))
         {
            METHOD(DualPipe, getExitCode) = Class_findMethod(CO(DualPipe), "GetExitCode", app);
            if(METHOD(DualPipe, getExitCode))
               DualPipe_getExitCode = (int (*)(C(DualPipe)))METHOD(DualPipe, getExitCode)->function;

            METHOD(DualPipe, getLinePeek) = Class_findMethod(CO(DualPipe), "GetLinePeek", app);
            if(METHOD(DualPipe, getLinePeek))
               DualPipe_getLinePeek = (C(bool) (*)(C(DualPipe), char *, int, int *))METHOD(DualPipe, getLinePeek)->function;

            METHOD(DualPipe, getProcessID) = Class_findMethod(CO(DualPipe), "GetProcessID", app);
            if(METHOD(DualPipe, getProcessID))
               DualPipe_getProcessID = (int (*)(C(DualPipe)))METHOD(DualPipe, getProcessID)->function;

            METHOD(DualPipe, peek) = Class_findMethod(CO(DualPipe), "Peek", app);
            if(METHOD(DualPipe, peek))
               DualPipe_peek = (C(bool) (*)(C(DualPipe)))METHOD(DualPipe, peek)->function;

            METHOD(DualPipe, terminate) = Class_findMethod(CO(DualPipe), "Terminate", app);
            if(METHOD(DualPipe, terminate))
               DualPipe_terminate = (void (*)(C(DualPipe)))METHOD(DualPipe, terminate)->function;

            METHOD(DualPipe, wait) = Class_findMethod(CO(DualPipe), "Wait", app);
            if(METHOD(DualPipe, wait))
               DualPipe_wait = (void (*)(C(DualPipe)))METHOD(DualPipe, wait)->function;
         }
         CO(ErrorCode) = eC_findClass(app, "ErrorCode");
         CO(ErrorLevel) = eC_findClass(app, "ErrorLevel");
         CO(File) = eC_findClass(app, "File");
         if(CO(File))
         {
            METHOD(File, close) = Class_findMethod(CO(File), "Close", app);
            if(METHOD(File, close))
               M_VTBLID(File, close) = METHOD(File, close)->vid;

            METHOD(File, closeInput) = Class_findMethod(CO(File), "CloseInput", app);
            if(METHOD(File, closeInput))
               M_VTBLID(File, closeInput) = METHOD(File, closeInput)->vid;

            METHOD(File, closeOutput) = Class_findMethod(CO(File), "CloseOutput", app);
            if(METHOD(File, closeOutput))
               M_VTBLID(File, closeOutput) = METHOD(File, closeOutput)->vid;

            METHOD(File, copyTo) = Class_findMethod(CO(File), "CopyTo", app);
            if(METHOD(File, copyTo))
               File_copyTo = (C(bool) (*)(C(File), const char *))METHOD(File, copyTo)->function;

            METHOD(File, copyToFile) = Class_findMethod(CO(File), "CopyToFile", app);
            if(METHOD(File, copyToFile))
               File_copyToFile = (C(bool) (*)(C(File), C(File)))METHOD(File, copyToFile)->function;

            METHOD(File, eof) = Class_findMethod(CO(File), "Eof", app);
            if(METHOD(File, eof))
               M_VTBLID(File, eof) = METHOD(File, eof)->vid;

            METHOD(File, flush) = Class_findMethod(CO(File), "Flush", app);
            if(METHOD(File, flush))
               File_flush = (C(bool) (*)(C(File)))METHOD(File, flush)->function;

            METHOD(File, getDouble) = Class_findMethod(CO(File), "GetDouble", app);
            if(METHOD(File, getDouble))
               File_getDouble = (double (*)(C(File)))METHOD(File, getDouble)->function;

            METHOD(File, getFloat) = Class_findMethod(CO(File), "GetFloat", app);
            if(METHOD(File, getFloat))
               File_getFloat = (float (*)(C(File)))METHOD(File, getFloat)->function;

            METHOD(File, getHexValue) = Class_findMethod(CO(File), "GetHexValue", app);
            if(METHOD(File, getHexValue))
               File_getHexValue = (uint (*)(C(File)))METHOD(File, getHexValue)->function;

            METHOD(File, getLine) = Class_findMethod(CO(File), "GetLine", app);
            if(METHOD(File, getLine))
               File_getLine = (C(bool) (*)(C(File), char *, int))METHOD(File, getLine)->function;

            METHOD(File, getLineEx) = Class_findMethod(CO(File), "GetLineEx", app);
            if(METHOD(File, getLineEx))
               File_getLineEx = (int (*)(C(File), char *, int, C(bool) *))METHOD(File, getLineEx)->function;

            METHOD(File, getSize) = Class_findMethod(CO(File), "GetSize", app);
            if(METHOD(File, getSize))
               M_VTBLID(File, getSize) = METHOD(File, getSize)->vid;

            METHOD(File, getString) = Class_findMethod(CO(File), "GetString", app);
            if(METHOD(File, getString))
               File_getString = (C(bool) (*)(C(File), char *, int))METHOD(File, getString)->function;

            METHOD(File, getValue) = Class_findMethod(CO(File), "GetValue", app);
            if(METHOD(File, getValue))
               File_getValue = (int (*)(C(File)))METHOD(File, getValue)->function;

            METHOD(File, getc) = Class_findMethod(CO(File), "Getc", app);
            if(METHOD(File, getc))
               M_VTBLID(File, getc) = METHOD(File, getc)->vid;

            METHOD(File, lock) = Class_findMethod(CO(File), "Lock", app);
            if(METHOD(File, lock))
               M_VTBLID(File, lock) = METHOD(File, lock)->vid;

            METHOD(File, print) = Class_findMethod(CO(File), "Print", app);
            if(METHOD(File, print))
               File_print = (void (*)(C(File), typed_object_class_ptr, const void *, ...))METHOD(File, print)->function;

            METHOD(File, printLn) = Class_findMethod(CO(File), "PrintLn", app);
            if(METHOD(File, printLn))
               File_printLn = (void (*)(C(File), typed_object_class_ptr, const void *, ...))METHOD(File, printLn)->function;

            METHOD(File, printf) = Class_findMethod(CO(File), "Printf", app);
            if(METHOD(File, printf))
               File_printf = (int (*)(C(File), const char *, ...))METHOD(File, printf)->function;

            METHOD(File, putc) = Class_findMethod(CO(File), "Putc", app);
            if(METHOD(File, putc))
               M_VTBLID(File, putc) = METHOD(File, putc)->vid;

            METHOD(File, puts) = Class_findMethod(CO(File), "Puts", app);
            if(METHOD(File, puts))
               M_VTBLID(File, puts) = METHOD(File, puts)->vid;

            METHOD(File, read) = Class_findMethod(CO(File), "Read", app);
            if(METHOD(File, read))
               M_VTBLID(File, read) = METHOD(File, read)->vid;

            METHOD(File, seek) = Class_findMethod(CO(File), "Seek", app);
            if(METHOD(File, seek))
               M_VTBLID(File, seek) = METHOD(File, seek)->vid;

            METHOD(File, tell) = Class_findMethod(CO(File), "Tell", app);
            if(METHOD(File, tell))
               M_VTBLID(File, tell) = METHOD(File, tell)->vid;

            METHOD(File, truncate) = Class_findMethod(CO(File), "Truncate", app);
            if(METHOD(File, truncate))
               M_VTBLID(File, truncate) = METHOD(File, truncate)->vid;

            METHOD(File, unlock) = Class_findMethod(CO(File), "Unlock", app);
            if(METHOD(File, unlock))
               M_VTBLID(File, unlock) = METHOD(File, unlock)->vid;

            METHOD(File, write) = Class_findMethod(CO(File), "Write", app);
            if(METHOD(File, write))
               M_VTBLID(File, write) = METHOD(File, write)->vid;

            PROPERTY(File, input) = Class_findProperty(CO(File), "input", app);
            if(PROPERTY(File, input))
            {
               File_set_input = (void *)PROPERTY(File, input)->Set;
               File_get_input = (void *)PROPERTY(File, input)->Get;
            }

            PROPERTY(File, output) = Class_findProperty(CO(File), "output", app);
            if(PROPERTY(File, output))
            {
               File_set_output = (void *)PROPERTY(File, output)->Set;
               File_get_output = (void *)PROPERTY(File, output)->Get;
            }

            PROPERTY(File, buffered) = Class_findProperty(CO(File), "buffered", app);
            if(PROPERTY(File, buffered))
               File_set_buffered = (void *)PROPERTY(File, buffered)->Set;

            PROPERTY(File, eof) = Class_findProperty(CO(File), "eof", app);
            if(PROPERTY(File, eof))
               File_get_eof = (void *)PROPERTY(File, eof)->Get;
         }
         CO(FileAttribs) = eC_findClass(app, "FileAttribs");
         CO(FileChange) = eC_findClass(app, "FileChange");
         CO(FileListing) = eC_findClass(app, "FileListing");
         if(CO(FileListing))
         {
            METHOD(FileListing, find) = Class_findMethod(CO(FileListing), "Find", app);
            if(METHOD(FileListing, find))
               FileListing_find = (C(bool) (*)(C(FileListing) *))METHOD(FileListing, find)->function;

            METHOD(FileListing, stop) = Class_findMethod(CO(FileListing), "Stop", app);
            if(METHOD(FileListing, stop))
               FileListing_stop = (void (*)(C(FileListing) *))METHOD(FileListing, stop)->function;

            PROPERTY(FileListing, name) = Class_findProperty(CO(FileListing), "name", app);
            if(PROPERTY(FileListing, name))
               FileListing_get_name = (void *)PROPERTY(FileListing, name)->Get;

            PROPERTY(FileListing, path) = Class_findProperty(CO(FileListing), "path", app);
            if(PROPERTY(FileListing, path))
               FileListing_get_path = (void *)PROPERTY(FileListing, path)->Get;

            PROPERTY(FileListing, stats) = Class_findProperty(CO(FileListing), "stats", app);
            if(PROPERTY(FileListing, stats))
               FileListing_get_stats = (void *)PROPERTY(FileListing, stats)->Get;
         }
         CO(FileLock) = eC_findClass(app, "FileLock");
         CO(FileMonitor) = eC_findClass(app, "FileMonitor");
         if(CO(FileMonitor))
         {
            METHOD(FileMonitor, onDirNotify) = Class_findMethod(CO(FileMonitor), "OnDirNotify", app);
            if(METHOD(FileMonitor, onDirNotify))
               M_VTBLID(FileMonitor, onDirNotify) = METHOD(FileMonitor, onDirNotify)->vid;

            METHOD(FileMonitor, onFileNotify) = Class_findMethod(CO(FileMonitor), "OnFileNotify", app);
            if(METHOD(FileMonitor, onFileNotify))
               M_VTBLID(FileMonitor, onFileNotify) = METHOD(FileMonitor, onFileNotify)->vid;

            METHOD(FileMonitor, startMonitoring) = Class_findMethod(CO(FileMonitor), "StartMonitoring", app);
            if(METHOD(FileMonitor, startMonitoring))
               FileMonitor_startMonitoring = (void (*)(C(FileMonitor)))METHOD(FileMonitor, startMonitoring)->function;

            METHOD(FileMonitor, stopMonitoring) = Class_findMethod(CO(FileMonitor), "StopMonitoring", app);
            if(METHOD(FileMonitor, stopMonitoring))
               FileMonitor_stopMonitoring = (void (*)(C(FileMonitor)))METHOD(FileMonitor, stopMonitoring)->function;

            PROPERTY(FileMonitor, userData) = Class_findProperty(CO(FileMonitor), "userData", app);
            if(PROPERTY(FileMonitor, userData))
               FileMonitor_set_userData = (void *)PROPERTY(FileMonitor, userData)->Set;

            PROPERTY(FileMonitor, fileChange) = Class_findProperty(CO(FileMonitor), "fileChange", app);
            if(PROPERTY(FileMonitor, fileChange))
               FileMonitor_set_fileChange = (void *)PROPERTY(FileMonitor, fileChange)->Set;

            PROPERTY(FileMonitor, fileName) = Class_findProperty(CO(FileMonitor), "fileName", app);
            if(PROPERTY(FileMonitor, fileName))
            {
               FileMonitor_set_fileName = (void *)PROPERTY(FileMonitor, fileName)->Set;
               FileMonitor_get_fileName = (void *)PROPERTY(FileMonitor, fileName)->Get;
            }

            PROPERTY(FileMonitor, directoryName) = Class_findProperty(CO(FileMonitor), "directoryName", app);
            if(PROPERTY(FileMonitor, directoryName))
            {
               FileMonitor_set_directoryName = (void *)PROPERTY(FileMonitor, directoryName)->Set;
               FileMonitor_get_directoryName = (void *)PROPERTY(FileMonitor, directoryName)->Get;
            }
         }
         CO(FileOpenMode) = eC_findClass(app, "FileOpenMode");
         CO(FileSeekMode) = eC_findClass(app, "FileSeekMode");
         CO(FileSize) = eC_findClass(app, "FileSize");
         CO(FileSize64) = eC_findClass(app, "FileSize64");
         CO(FileStats) = eC_findClass(app, "FileStats");
         CO(GuiErrorCode) = eC_findClass(app, "GuiErrorCode");
         CO(LoggingMode) = eC_findClass(app, "LoggingMode");
         CO(MoveFileOptions) = eC_findClass(app, "MoveFileOptions");
         CO(PipeOpenMode) = eC_findClass(app, "PipeOpenMode");
         CO(SysErrorCode) = eC_findClass(app, "SysErrorCode");
         CO(TempFile) = eC_findClass(app, "TempFile");
         if(CO(TempFile))
         {
            METHOD(TempFile, stealBuffer) = Class_findMethod(CO(TempFile), "StealBuffer", app);
            if(METHOD(TempFile, stealBuffer))
               TempFile_stealBuffer = (byte * (*)(C(TempFile)))METHOD(TempFile, stealBuffer)->function;

            PROPERTY(TempFile, openMode) = Class_findProperty(CO(TempFile), "openMode", app);
            if(PROPERTY(TempFile, openMode))
            {
               TempFile_set_openMode = (void *)PROPERTY(TempFile, openMode)->Set;
               TempFile_get_openMode = (void *)PROPERTY(TempFile, openMode)->Get;
            }

            PROPERTY(TempFile, buffer) = Class_findProperty(CO(TempFile), "buffer", app);
            if(PROPERTY(TempFile, buffer))
            {
               TempFile_set_buffer = (void *)PROPERTY(TempFile, buffer)->Set;
               TempFile_get_buffer = (void *)PROPERTY(TempFile, buffer)->Get;
            }

            PROPERTY(TempFile, size) = Class_findProperty(CO(TempFile), "size", app);
            if(PROPERTY(TempFile, size))
            {
               TempFile_set_size = (void *)PROPERTY(TempFile, size)->Set;
               TempFile_get_size = (void *)PROPERTY(TempFile, size)->Get;
            }

            PROPERTY(TempFile, allocated) = Class_findProperty(CO(TempFile), "allocated", app);
            if(PROPERTY(TempFile, allocated))
            {
               TempFile_set_allocated = (void *)PROPERTY(TempFile, allocated)->Set;
               TempFile_get_allocated = (void *)PROPERTY(TempFile, allocated)->Get;
            }
         }
         CO(CharCategories) = eC_findClass(app, "CharCategories");
         CO(CharCategory) = eC_findClass(app, "CharCategory");
         CO(PredefinedCharCategories) = eC_findClass(app, "PredefinedCharCategories");
         CO(UnicodeDecomposition) = eC_findClass(app, "UnicodeDecomposition");
         CO(ECONGlobalSettings) = eC_findClass(app, "ECONGlobalSettings");
         CO(ECONParser) = eC_findClass(app, "ECONParser");
         CO(GlobalAppSettings) = eC_findClass(app, "GlobalAppSettings");
         if(CO(GlobalAppSettings))
         {
            METHOD(GlobalAppSettings, getGlobalValue) = Class_findMethod(CO(GlobalAppSettings), "GetGlobalValue", app);
            if(METHOD(GlobalAppSettings, getGlobalValue))
               GlobalAppSettings_getGlobalValue = (C(bool) (*)(C(GlobalAppSettings), const char *, const char *, C(GlobalSettingType), void *))METHOD(GlobalAppSettings, getGlobalValue)->function;

            METHOD(GlobalAppSettings, putGlobalValue) = Class_findMethod(CO(GlobalAppSettings), "PutGlobalValue", app);
            if(METHOD(GlobalAppSettings, putGlobalValue))
               GlobalAppSettings_putGlobalValue = (C(bool) (*)(C(GlobalAppSettings), const char *, const char *, C(GlobalSettingType), const void *))METHOD(GlobalAppSettings, putGlobalValue)->function;
         }
         CO(GlobalSettingType) = eC_findClass(app, "GlobalSettingType");
         CO(GlobalSettings) = eC_findClass(app, "GlobalSettings");
         if(CO(GlobalSettings))
         {
            METHOD(GlobalSettings, close) = Class_findMethod(CO(GlobalSettings), "Close", app);
            if(METHOD(GlobalSettings, close))
               GlobalSettings_close = (void (*)(C(GlobalSettings)))METHOD(GlobalSettings, close)->function;

            METHOD(GlobalSettings, closeAndMonitor) = Class_findMethod(CO(GlobalSettings), "CloseAndMonitor", app);
            if(METHOD(GlobalSettings, closeAndMonitor))
               GlobalSettings_closeAndMonitor = (void (*)(C(GlobalSettings)))METHOD(GlobalSettings, closeAndMonitor)->function;

            METHOD(GlobalSettings, load) = Class_findMethod(CO(GlobalSettings), "Load", app);
            if(METHOD(GlobalSettings, load))
               M_VTBLID(GlobalSettings, load) = METHOD(GlobalSettings, load)->vid;

            METHOD(GlobalSettings, onAskReloadSettings) = Class_findMethod(CO(GlobalSettings), "OnAskReloadSettings", app);
            if(METHOD(GlobalSettings, onAskReloadSettings))
               M_VTBLID(GlobalSettings, onAskReloadSettings) = METHOD(GlobalSettings, onAskReloadSettings)->vid;

            METHOD(GlobalSettings, openAndLock) = Class_findMethod(CO(GlobalSettings), "OpenAndLock", app);
            if(METHOD(GlobalSettings, openAndLock))
               GlobalSettings_openAndLock = (C(bool) (*)(C(GlobalSettings), C(FileSize) *))METHOD(GlobalSettings, openAndLock)->function;

            METHOD(GlobalSettings, save) = Class_findMethod(CO(GlobalSettings), "Save", app);
            if(METHOD(GlobalSettings, save))
               M_VTBLID(GlobalSettings, save) = METHOD(GlobalSettings, save)->vid;

            PROPERTY(GlobalSettings, settingsName) = Class_findProperty(CO(GlobalSettings), "settingsName", app);
            if(PROPERTY(GlobalSettings, settingsName))
            {
               GlobalSettings_set_settingsName = (void *)PROPERTY(GlobalSettings, settingsName)->Set;
               GlobalSettings_get_settingsName = (void *)PROPERTY(GlobalSettings, settingsName)->Get;
            }

            PROPERTY(GlobalSettings, settingsExtension) = Class_findProperty(CO(GlobalSettings), "settingsExtension", app);
            if(PROPERTY(GlobalSettings, settingsExtension))
            {
               GlobalSettings_set_settingsExtension = (void *)PROPERTY(GlobalSettings, settingsExtension)->Set;
               GlobalSettings_get_settingsExtension = (void *)PROPERTY(GlobalSettings, settingsExtension)->Get;
            }

            PROPERTY(GlobalSettings, settingsDirectory) = Class_findProperty(CO(GlobalSettings), "settingsDirectory", app);
            if(PROPERTY(GlobalSettings, settingsDirectory))
            {
               GlobalSettings_set_settingsDirectory = (void *)PROPERTY(GlobalSettings, settingsDirectory)->Set;
               GlobalSettings_get_settingsDirectory = (void *)PROPERTY(GlobalSettings, settingsDirectory)->Get;
            }

            PROPERTY(GlobalSettings, settingsLocation) = Class_findProperty(CO(GlobalSettings), "settingsLocation", app);
            if(PROPERTY(GlobalSettings, settingsLocation))
            {
               GlobalSettings_set_settingsLocation = (void *)PROPERTY(GlobalSettings, settingsLocation)->Set;
               GlobalSettings_get_settingsLocation = (void *)PROPERTY(GlobalSettings, settingsLocation)->Get;
            }

            PROPERTY(GlobalSettings, settingsFilePath) = Class_findProperty(CO(GlobalSettings), "settingsFilePath", app);
            if(PROPERTY(GlobalSettings, settingsFilePath))
            {
               GlobalSettings_set_settingsFilePath = (void *)PROPERTY(GlobalSettings, settingsFilePath)->Set;
               GlobalSettings_get_settingsFilePath = (void *)PROPERTY(GlobalSettings, settingsFilePath)->Get;
            }

            PROPERTY(GlobalSettings, allowDefaultLocations) = Class_findProperty(CO(GlobalSettings), "allowDefaultLocations", app);
            if(PROPERTY(GlobalSettings, allowDefaultLocations))
            {
               GlobalSettings_set_allowDefaultLocations = (void *)PROPERTY(GlobalSettings, allowDefaultLocations)->Set;
               GlobalSettings_get_allowDefaultLocations = (void *)PROPERTY(GlobalSettings, allowDefaultLocations)->Get;
            }

            PROPERTY(GlobalSettings, allUsers) = Class_findProperty(CO(GlobalSettings), "allUsers", app);
            if(PROPERTY(GlobalSettings, allUsers))
            {
               GlobalSettings_set_allUsers = (void *)PROPERTY(GlobalSettings, allUsers)->Set;
               GlobalSettings_get_allUsers = (void *)PROPERTY(GlobalSettings, allUsers)->Get;
            }

            PROPERTY(GlobalSettings, portable) = Class_findProperty(CO(GlobalSettings), "portable", app);
            if(PROPERTY(GlobalSettings, portable))
            {
               GlobalSettings_set_portable = (void *)PROPERTY(GlobalSettings, portable)->Set;
               GlobalSettings_get_portable = (void *)PROPERTY(GlobalSettings, portable)->Get;
            }

            PROPERTY(GlobalSettings, driver) = Class_findProperty(CO(GlobalSettings), "driver", app);
            if(PROPERTY(GlobalSettings, driver))
            {
               GlobalSettings_set_driver = (void *)PROPERTY(GlobalSettings, driver)->Set;
               GlobalSettings_get_driver = (void *)PROPERTY(GlobalSettings, driver)->Get;
            }

            PROPERTY(GlobalSettings, isGlobalPath) = Class_findProperty(CO(GlobalSettings), "isGlobalPath", app);
            if(PROPERTY(GlobalSettings, isGlobalPath))
               GlobalSettings_get_isGlobalPath = (void *)PROPERTY(GlobalSettings, isGlobalPath)->Get;
         }
         CO(GlobalSettingsData) = eC_findClass(app, "GlobalSettingsData");
         CO(GlobalSettingsDriver) = eC_findClass(app, "GlobalSettingsDriver");
         if(CO(GlobalSettingsDriver))
         {
            METHOD(GlobalSettingsDriver, load) = Class_findMethod(CO(GlobalSettingsDriver), "Load", app);
            if(METHOD(GlobalSettingsDriver, load))
               M_VTBLID(GlobalSettingsDriver, load) = METHOD(GlobalSettingsDriver, load)->vid;

            METHOD(GlobalSettingsDriver, save) = Class_findMethod(CO(GlobalSettingsDriver), "Save", app);
            if(METHOD(GlobalSettingsDriver, save))
               M_VTBLID(GlobalSettingsDriver, save) = METHOD(GlobalSettingsDriver, save)->vid;
         }
         CO(JSONFirstLetterCapitalization) = eC_findClass(app, "JSONFirstLetterCapitalization");
         CO(JSONGlobalSettings) = eC_findClass(app, "JSONGlobalSettings");
         CO(JSONParser) = eC_findClass(app, "JSONParser");
         if(CO(JSONParser))
         {
            METHOD(JSONParser, getObject) = Class_findMethod(CO(JSONParser), "GetObject", app);
            if(METHOD(JSONParser, getObject))
               JSONParser_getObject = (C(JSONResult) (*)(C(JSONParser), C(Class) *, void **))METHOD(JSONParser, getObject)->function;

            PROPERTY(JSONParser, debug) = Class_findProperty(CO(JSONParser), "debug", app);
            if(PROPERTY(JSONParser, debug))
            {
               JSONParser_set_debug = (void *)PROPERTY(JSONParser, debug)->Set;
               JSONParser_get_debug = (void *)PROPERTY(JSONParser, debug)->Get;
            }

            PROPERTY(JSONParser, warnings) = Class_findProperty(CO(JSONParser), "warnings", app);
            if(PROPERTY(JSONParser, warnings))
            {
               JSONParser_set_warnings = (void *)PROPERTY(JSONParser, warnings)->Set;
               JSONParser_get_warnings = (void *)PROPERTY(JSONParser, warnings)->Get;
            }
         }
         CO(JSONResult) = eC_findClass(app, "JSONResult");
         CO(JSONTypeOptions) = eC_findClass(app, "JSONTypeOptions");
         CO(OptionsMap) = eC_findClass(app, "OptionsMap");
         CO(SetBool) = eC_findClass(app, "SetBool");
         CO(SettingsIOResult) = eC_findClass(app, "SettingsIOResult");
         CO(Condition) = eC_findClass(app, "Condition");
         if(CO(Condition))
         {
            METHOD(Condition, signal) = Class_findMethod(CO(Condition), "Signal", app);
            if(METHOD(Condition, signal))
               Condition_signal = (void (*)(C(Condition) *))METHOD(Condition, signal)->function;

            METHOD(Condition, wait) = Class_findMethod(CO(Condition), "Wait", app);
            if(METHOD(Condition, wait))
               Condition_wait = (void (*)(C(Condition) *, C(Mutex) *))METHOD(Condition, wait)->function;

            PROPERTY(Condition, name) = Class_findProperty(CO(Condition), "name", app);
            if(PROPERTY(Condition, name))
            {
               Condition_set_name = (void *)PROPERTY(Condition, name)->Set;
               Condition_get_name = (void *)PROPERTY(Condition, name)->Get;
            }
         }
         CO(Mutex) = eC_findClass(app, "Mutex");
         if(CO(Mutex))
         {
            METHOD(Mutex, release) = Class_findMethod(CO(Mutex), "Release", app);
            if(METHOD(Mutex, release))
               Mutex_release = (void (*)(C(Mutex) *))METHOD(Mutex, release)->function;

            METHOD(Mutex, wait) = Class_findMethod(CO(Mutex), "Wait", app);
            if(METHOD(Mutex, wait))
               Mutex_wait = (void (*)(C(Mutex) *))METHOD(Mutex, wait)->function;

            PROPERTY(Mutex, lockCount) = Class_findProperty(CO(Mutex), "lockCount", app);
            if(PROPERTY(Mutex, lockCount))
               Mutex_get_lockCount = (void *)PROPERTY(Mutex, lockCount)->Get;

            PROPERTY(Mutex, owningThread) = Class_findProperty(CO(Mutex), "owningThread", app);
            if(PROPERTY(Mutex, owningThread))
               Mutex_get_owningThread = (void *)PROPERTY(Mutex, owningThread)->Get;
         }
         CO(Semaphore) = eC_findClass(app, "Semaphore");
         if(CO(Semaphore))
         {
            METHOD(Semaphore, release) = Class_findMethod(CO(Semaphore), "Release", app);
            if(METHOD(Semaphore, release))
               Semaphore_release = (void (*)(C(Semaphore) *))METHOD(Semaphore, release)->function;

            METHOD(Semaphore, tryWait) = Class_findMethod(CO(Semaphore), "TryWait", app);
            if(METHOD(Semaphore, tryWait))
               Semaphore_tryWait = (C(bool) (*)(C(Semaphore) *))METHOD(Semaphore, tryWait)->function;

            METHOD(Semaphore, wait) = Class_findMethod(CO(Semaphore), "Wait", app);
            if(METHOD(Semaphore, wait))
               Semaphore_wait = (void (*)(C(Semaphore) *))METHOD(Semaphore, wait)->function;

            PROPERTY(Semaphore, initCount) = Class_findProperty(CO(Semaphore), "initCount", app);
            if(PROPERTY(Semaphore, initCount))
            {
               Semaphore_set_initCount = (void *)PROPERTY(Semaphore, initCount)->Set;
               Semaphore_get_initCount = (void *)PROPERTY(Semaphore, initCount)->Get;
            }

            PROPERTY(Semaphore, maxCount) = Class_findProperty(CO(Semaphore), "maxCount", app);
            if(PROPERTY(Semaphore, maxCount))
            {
               Semaphore_set_maxCount = (void *)PROPERTY(Semaphore, maxCount)->Set;
               Semaphore_get_maxCount = (void *)PROPERTY(Semaphore, maxCount)->Get;
            }
         }
         CO(Thread) = eC_findClass(app, "Thread");
         if(CO(Thread))
         {
            METHOD(Thread, create) = Class_findMethod(CO(Thread), "Create", app);
            if(METHOD(Thread, create))
               Thread_create = (void (*)(C(Thread)))METHOD(Thread, create)->function;

            METHOD(Thread, kill) = Class_findMethod(CO(Thread), "Kill", app);
            if(METHOD(Thread, kill))
               Thread_kill = (void (*)(C(Thread)))METHOD(Thread, kill)->function;

            METHOD(Thread, main) = Class_findMethod(CO(Thread), "Main", app);
            if(METHOD(Thread, main))
               M_VTBLID(Thread, main) = METHOD(Thread, main)->vid;

            METHOD(Thread, setPriority) = Class_findMethod(CO(Thread), "SetPriority", app);
            if(METHOD(Thread, setPriority))
               Thread_setPriority = (void (*)(C(Thread), C(ThreadPriority)))METHOD(Thread, setPriority)->function;

            METHOD(Thread, wait) = Class_findMethod(CO(Thread), "Wait", app);
            if(METHOD(Thread, wait))
               Thread_wait = (void (*)(C(Thread)))METHOD(Thread, wait)->function;

            PROPERTY(Thread, created) = Class_findProperty(CO(Thread), "created", app);
            if(PROPERTY(Thread, created))
               Thread_get_created = (void *)PROPERTY(Thread, created)->Get;
         }
         CO(ThreadPriority) = eC_findClass(app, "ThreadPriority");
         CO(Date) = eC_findClass(app, "Date");
         if(CO(Date))
         {
            METHOD(Date, onGetStringEn) = Class_findMethod(CO(Date), "OnGetStringEn", app);
            if(METHOD(Date, onGetStringEn))
               Date_onGetStringEn = (const char * (*)(C(Date) *, char *, void *, C(ObjectNotationType) *))METHOD(Date, onGetStringEn)->function;

            PROPERTY(Date, dayOfTheWeek) = Class_findProperty(CO(Date), "dayOfTheWeek", app);
            if(PROPERTY(Date, dayOfTheWeek))
               Date_get_dayOfTheWeek = (void *)PROPERTY(Date, dayOfTheWeek)->Get;
         }
         CO(DateTime) = eC_findClass(app, "DateTime");
         if(CO(DateTime))
         {
            METHOD(DateTime, fixDayOfYear) = Class_findMethod(CO(DateTime), "FixDayOfYear", app);
            if(METHOD(DateTime, fixDayOfYear))
               DateTime_fixDayOfYear = (C(bool) (*)(C(DateTime) *))METHOD(DateTime, fixDayOfYear)->function;

            METHOD(DateTime, getLocalTime) = Class_findMethod(CO(DateTime), "GetLocalTime", app);
            if(METHOD(DateTime, getLocalTime))
               DateTime_getLocalTime = (C(bool) (*)(C(DateTime) *))METHOD(DateTime, getLocalTime)->function;

            PROPERTY(DateTime, SecSince1970) = Class_findProperty(CO(DateTime), "eC::time::SecSince1970", app);
            if(PROPERTY(DateTime, SecSince1970))
            {
               DateTime_from_SecSince1970 = (void *)PROPERTY(DateTime, SecSince1970)->Set;
               DateTime_to_SecSince1970 = (void *)PROPERTY(DateTime, SecSince1970)->Get;
            }

            PROPERTY(DateTime, Date) = Class_findProperty(CO(DateTime), "eC::time::Date", app);
            if(PROPERTY(DateTime, Date))
            {
               DateTime_from_Date = (void *)PROPERTY(DateTime, Date)->Set;
               DateTime_to_Date = (void *)PROPERTY(DateTime, Date)->Get;
            }

            PROPERTY(DateTime, global) = Class_findProperty(CO(DateTime), "global", app);
            if(PROPERTY(DateTime, global))
            {
               DateTime_set_global = (void *)PROPERTY(DateTime, global)->Set;
               DateTime_get_global = (void *)PROPERTY(DateTime, global)->Get;
            }

            PROPERTY(DateTime, local) = Class_findProperty(CO(DateTime), "local", app);
            if(PROPERTY(DateTime, local))
            {
               DateTime_set_local = (void *)PROPERTY(DateTime, local)->Set;
               DateTime_get_local = (void *)PROPERTY(DateTime, local)->Get;
            }

            PROPERTY(DateTime, daysSince1970) = Class_findProperty(CO(DateTime), "daysSince1970", app);
            if(PROPERTY(DateTime, daysSince1970))
               DateTime_get_daysSince1970 = (void *)PROPERTY(DateTime, daysSince1970)->Get;
         }
         CO(DayOfTheWeek) = eC_findClass(app, "DayOfTheWeek");
         CO(Month) = eC_findClass(app, "Month");
         if(CO(Month))
         {
            METHOD(Month, getNumDays) = Class_findMethod(CO(Month), "getNumDays", app);
            if(METHOD(Month, getNumDays))
               Month_getNumDays = (int (*)(C(Month), int))METHOD(Month, getNumDays)->function;
         }
         CO(SecSince1970) = eC_findClass(app, "SecSince1970");
         if(CO(SecSince1970))
         {
            PROPERTY(SecSince1970, global) = Class_findProperty(CO(SecSince1970), "global", app);
            if(PROPERTY(SecSince1970, global))
               SecSince1970_get_global = (void *)PROPERTY(SecSince1970, global)->Get;

            PROPERTY(SecSince1970, local) = Class_findProperty(CO(SecSince1970), "local", app);
            if(PROPERTY(SecSince1970, local))
               SecSince1970_get_local = (void *)PROPERTY(SecSince1970, local)->Get;
         }
         CO(Seconds) = eC_findClass(app, "Seconds");
         if(CO(Seconds))
         {
         }
         CO(Time) = eC_findClass(app, "Time");
         CO(TimeStamp) = eC_findClass(app, "TimeStamp");
         if(CO(TimeStamp))
         {
         }
         CO(TimeStamp32) = eC_findClass(app, "TimeStamp32");
         CO(AccessMode) = eC_findClass(app, "AccessMode");
         CO(Angle) = eC_findClass(app, "Angle");
         CO(BTNamedLink) = eC_findClass(app, "BTNamedLink");
         CO(BackSlashEscaping) = eC_findClass(app, "BackSlashEscaping");
         CO(BitMember) = eC_findClass(app, "BitMember");
         CO(Box) = eC_findClass(app, "Box");
         if(CO(Box))
         {
            METHOD(Box, clip) = Class_findMethod(CO(Box), "Clip", app);
            if(METHOD(Box, clip))
               Box_clip = (void (*)(C(Box) *, C(Box) *))METHOD(Box, clip)->function;

            METHOD(Box, clipOffset) = Class_findMethod(CO(Box), "ClipOffset", app);
            if(METHOD(Box, clipOffset))
               Box_clipOffset = (void (*)(C(Box) *, C(Box) *, int, int))METHOD(Box, clipOffset)->function;

            METHOD(Box, isPointInside) = Class_findMethod(CO(Box), "IsPointInside", app);
            if(METHOD(Box, isPointInside))
               Box_isPointInside = (C(bool) (*)(C(Box) *, C(Point) *))METHOD(Box, isPointInside)->function;

            METHOD(Box, overlap) = Class_findMethod(CO(Box), "Overlap", app);
            if(METHOD(Box, overlap))
               Box_overlap = (C(bool) (*)(C(Box) *, C(Box) *))METHOD(Box, overlap)->function;

            PROPERTY(Box, width) = Class_findProperty(CO(Box), "width", app);
            if(PROPERTY(Box, width))
            {
               Box_set_width = (void *)PROPERTY(Box, width)->Set;
               Box_get_width = (void *)PROPERTY(Box, width)->Get;
            }

            PROPERTY(Box, height) = Class_findProperty(CO(Box), "height", app);
            if(PROPERTY(Box, height))
            {
               Box_set_height = (void *)PROPERTY(Box, height)->Set;
               Box_get_height = (void *)PROPERTY(Box, height)->Get;
            }
         }
         CO(CIString) = eC_findClass(app, "CIString");
         CO(Centimeters) = eC_findClass(app, "Centimeters");
         if(CO(Centimeters))
         {
            PROPERTY(Centimeters, Meters) = Class_findProperty(CO(Centimeters), "eC::types::Meters", app);
            if(PROPERTY(Centimeters, Meters))
            {
               Centimeters_from_Meters = (void *)PROPERTY(Centimeters, Meters)->Set;
               Centimeters_to_Meters = (void *)PROPERTY(Centimeters, Meters)->Get;
            }
         }
         CO(Class) = eC_findClass(app, "Class");
         if(CO(Class))
         {
            PROPERTY(Class, char_ptr) = Class_findProperty(CO(Class), "char *", app);
            if(PROPERTY(Class, char_ptr))
            {
               Class_from_char_ptr = (void *)PROPERTY(Class, char_ptr)->Set;
               Class_to_char_ptr = (void *)PROPERTY(Class, char_ptr)->Get;
            }
         }
         CO(ClassDesignerBase) = eC_findClass(app, "ClassDesignerBase");
         if(CO(ClassDesignerBase))
         {
            METHOD(ClassDesignerBase, addObject) = Class_findMethod(CO(ClassDesignerBase), "AddObject", app);
            if(METHOD(ClassDesignerBase, addObject))
               M_VTBLID(ClassDesignerBase, addObject) = METHOD(ClassDesignerBase, addObject)->vid;

            METHOD(ClassDesignerBase, createNew) = Class_findMethod(CO(ClassDesignerBase), "CreateNew", app);
            if(METHOD(ClassDesignerBase, createNew))
               M_VTBLID(ClassDesignerBase, createNew) = METHOD(ClassDesignerBase, createNew)->vid;

            METHOD(ClassDesignerBase, createObject) = Class_findMethod(CO(ClassDesignerBase), "CreateObject", app);
            if(METHOD(ClassDesignerBase, createObject))
               M_VTBLID(ClassDesignerBase, createObject) = METHOD(ClassDesignerBase, createObject)->vid;

            METHOD(ClassDesignerBase, destroyObject) = Class_findMethod(CO(ClassDesignerBase), "DestroyObject", app);
            if(METHOD(ClassDesignerBase, destroyObject))
               M_VTBLID(ClassDesignerBase, destroyObject) = METHOD(ClassDesignerBase, destroyObject)->vid;

            METHOD(ClassDesignerBase, droppedObject) = Class_findMethod(CO(ClassDesignerBase), "DroppedObject", app);
            if(METHOD(ClassDesignerBase, droppedObject))
               M_VTBLID(ClassDesignerBase, droppedObject) = METHOD(ClassDesignerBase, droppedObject)->vid;

            METHOD(ClassDesignerBase, fixProperty) = Class_findMethod(CO(ClassDesignerBase), "FixProperty", app);
            if(METHOD(ClassDesignerBase, fixProperty))
               M_VTBLID(ClassDesignerBase, fixProperty) = METHOD(ClassDesignerBase, fixProperty)->vid;

            METHOD(ClassDesignerBase, listToolBoxClasses) = Class_findMethod(CO(ClassDesignerBase), "ListToolBoxClasses", app);
            if(METHOD(ClassDesignerBase, listToolBoxClasses))
               M_VTBLID(ClassDesignerBase, listToolBoxClasses) = METHOD(ClassDesignerBase, listToolBoxClasses)->vid;

            METHOD(ClassDesignerBase, postCreateObject) = Class_findMethod(CO(ClassDesignerBase), "PostCreateObject", app);
            if(METHOD(ClassDesignerBase, postCreateObject))
               M_VTBLID(ClassDesignerBase, postCreateObject) = METHOD(ClassDesignerBase, postCreateObject)->vid;

            METHOD(ClassDesignerBase, prepareTestObject) = Class_findMethod(CO(ClassDesignerBase), "PrepareTestObject", app);
            if(METHOD(ClassDesignerBase, prepareTestObject))
               M_VTBLID(ClassDesignerBase, prepareTestObject) = METHOD(ClassDesignerBase, prepareTestObject)->vid;

            METHOD(ClassDesignerBase, reset) = Class_findMethod(CO(ClassDesignerBase), "Reset", app);
            if(METHOD(ClassDesignerBase, reset))
               M_VTBLID(ClassDesignerBase, reset) = METHOD(ClassDesignerBase, reset)->vid;

            METHOD(ClassDesignerBase, selectObject) = Class_findMethod(CO(ClassDesignerBase), "SelectObject", app);
            if(METHOD(ClassDesignerBase, selectObject))
               M_VTBLID(ClassDesignerBase, selectObject) = METHOD(ClassDesignerBase, selectObject)->vid;
         }
         CO(ClassProperty) = eC_findClass(app, "ClassProperty");
         CO(ClassTemplateArgument) = eC_findClass(app, "ClassTemplateArgument");
         CO(ClassTemplateParameter) = eC_findClass(app, "ClassTemplateParameter");
         CO(ClassType) = eC_findClass(app, "ClassType");
         CO(DataMember) = eC_findClass(app, "DataMember");
         CO(DataMemberType) = eC_findClass(app, "DataMemberType");
         CO(DataValue) = eC_findClass(app, "DataValue");
         CO(DefinedExpression) = eC_findClass(app, "DefinedExpression");
         CO(Degrees) = eC_findClass(app, "Degrees");
         if(CO(Degrees))
         {
            PROPERTY(Degrees, Radians) = Class_findProperty(CO(Degrees), "eC::types::Radians", app);
            if(PROPERTY(Degrees, Radians))
            {
               Degrees_from_Radians = (void *)PROPERTY(Degrees, Radians)->Set;
               Degrees_to_Radians = (void *)PROPERTY(Degrees, Radians)->Get;
            }
         }
         CO(DesignerBase) = eC_findClass(app, "DesignerBase");
         if(CO(DesignerBase))
         {
            METHOD(DesignerBase, addDefaultMethod) = Class_findMethod(CO(DesignerBase), "AddDefaultMethod", app);
            if(METHOD(DesignerBase, addDefaultMethod))
               M_VTBLID(DesignerBase, addDefaultMethod) = METHOD(DesignerBase, addDefaultMethod)->vid;

            METHOD(DesignerBase, addToolBoxClass) = Class_findMethod(CO(DesignerBase), "AddToolBoxClass", app);
            if(METHOD(DesignerBase, addToolBoxClass))
               M_VTBLID(DesignerBase, addToolBoxClass) = METHOD(DesignerBase, addToolBoxClass)->vid;

            METHOD(DesignerBase, codeAddObject) = Class_findMethod(CO(DesignerBase), "CodeAddObject", app);
            if(METHOD(DesignerBase, codeAddObject))
               M_VTBLID(DesignerBase, codeAddObject) = METHOD(DesignerBase, codeAddObject)->vid;

            METHOD(DesignerBase, deleteObject) = Class_findMethod(CO(DesignerBase), "DeleteObject", app);
            if(METHOD(DesignerBase, deleteObject))
               M_VTBLID(DesignerBase, deleteObject) = METHOD(DesignerBase, deleteObject)->vid;

            METHOD(DesignerBase, findObject) = Class_findMethod(CO(DesignerBase), "FindObject", app);
            if(METHOD(DesignerBase, findObject))
               M_VTBLID(DesignerBase, findObject) = METHOD(DesignerBase, findObject)->vid;

            METHOD(DesignerBase, modifyCode) = Class_findMethod(CO(DesignerBase), "ModifyCode", app);
            if(METHOD(DesignerBase, modifyCode))
               M_VTBLID(DesignerBase, modifyCode) = METHOD(DesignerBase, modifyCode)->vid;

            METHOD(DesignerBase, objectContainsCode) = Class_findMethod(CO(DesignerBase), "ObjectContainsCode", app);
            if(METHOD(DesignerBase, objectContainsCode))
               M_VTBLID(DesignerBase, objectContainsCode) = METHOD(DesignerBase, objectContainsCode)->vid;

            METHOD(DesignerBase, renameObject) = Class_findMethod(CO(DesignerBase), "RenameObject", app);
            if(METHOD(DesignerBase, renameObject))
               M_VTBLID(DesignerBase, renameObject) = METHOD(DesignerBase, renameObject)->vid;

            METHOD(DesignerBase, selectObjectFromDesigner) = Class_findMethod(CO(DesignerBase), "SelectObjectFromDesigner", app);
            if(METHOD(DesignerBase, selectObjectFromDesigner))
               M_VTBLID(DesignerBase, selectObjectFromDesigner) = METHOD(DesignerBase, selectObjectFromDesigner)->vid;

            METHOD(DesignerBase, sheetAddObject) = Class_findMethod(CO(DesignerBase), "SheetAddObject", app);
            if(METHOD(DesignerBase, sheetAddObject))
               M_VTBLID(DesignerBase, sheetAddObject) = METHOD(DesignerBase, sheetAddObject)->vid;

            METHOD(DesignerBase, updateProperties) = Class_findMethod(CO(DesignerBase), "UpdateProperties", app);
            if(METHOD(DesignerBase, updateProperties))
               M_VTBLID(DesignerBase, updateProperties) = METHOD(DesignerBase, updateProperties)->vid;

            PROPERTY(DesignerBase, classDesigner) = Class_findProperty(CO(DesignerBase), "classDesigner", app);
            if(PROPERTY(DesignerBase, classDesigner))
            {
               DesignerBase_set_classDesigner = (void *)PROPERTY(DesignerBase, classDesigner)->Set;
               DesignerBase_get_classDesigner = (void *)PROPERTY(DesignerBase, classDesigner)->Get;
            }

            PROPERTY(DesignerBase, objectClass) = Class_findProperty(CO(DesignerBase), "objectClass", app);
            if(PROPERTY(DesignerBase, objectClass))
            {
               DesignerBase_set_objectClass = (void *)PROPERTY(DesignerBase, objectClass)->Set;
               DesignerBase_get_objectClass = (void *)PROPERTY(DesignerBase, objectClass)->Get;
            }

            PROPERTY(DesignerBase, isDragging) = Class_findProperty(CO(DesignerBase), "isDragging", app);
            if(PROPERTY(DesignerBase, isDragging))
            {
               DesignerBase_set_isDragging = (void *)PROPERTY(DesignerBase, isDragging)->Set;
               DesignerBase_get_isDragging = (void *)PROPERTY(DesignerBase, isDragging)->Get;
            }
         }
         CO(Distance) = eC_findClass(app, "Distance");
         CO(EnumClassData) = eC_findClass(app, "EnumClassData");
         CO(EscapeCStringOptions) = eC_findClass(app, "EscapeCStringOptions");
         CO(Feet) = eC_findClass(app, "Feet");
         if(CO(Feet))
         {
            PROPERTY(Feet, Meters) = Class_findProperty(CO(Feet), "eC::types::Meters", app);
            if(PROPERTY(Feet, Meters))
            {
               Feet_from_Meters = (void *)PROPERTY(Feet, Meters)->Set;
               Feet_to_Meters = (void *)PROPERTY(Feet, Meters)->Get;
            }
         }
         CO(GlobalFunction) = eC_findClass(app, "GlobalFunction");
         CO(IOChannel) = eC_findClass(app, "IOChannel");
         if(CO(IOChannel))
         {
            METHOD(IOChannel, get) = Class_findMethod(CO(IOChannel), "Get", app);
            if(METHOD(IOChannel, get))
               IOChannel_get = (void (*)(C(IOChannel), typed_object_class_ptr, void *))METHOD(IOChannel, get)->function;

            METHOD(IOChannel, put) = Class_findMethod(CO(IOChannel), "Put", app);
            if(METHOD(IOChannel, put))
               IOChannel_put = (void (*)(C(IOChannel), typed_object_class_ptr, void *))METHOD(IOChannel, put)->function;

            METHOD(IOChannel, readData) = Class_findMethod(CO(IOChannel), "ReadData", app);
            if(METHOD(IOChannel, readData))
               M_VTBLID(IOChannel, readData) = METHOD(IOChannel, readData)->vid;

            METHOD(IOChannel, serialize) = Class_findMethod(CO(IOChannel), "Serialize", app);
            if(METHOD(IOChannel, serialize))
               IOChannel_serialize = (void (*)(C(IOChannel), typed_object_class_ptr, void *))METHOD(IOChannel, serialize)->function;

            METHOD(IOChannel, unserialize) = Class_findMethod(CO(IOChannel), "Unserialize", app);
            if(METHOD(IOChannel, unserialize))
               IOChannel_unserialize = (void (*)(C(IOChannel), typed_object_class_ptr, void *))METHOD(IOChannel, unserialize)->function;

            METHOD(IOChannel, writeData) = Class_findMethod(CO(IOChannel), "WriteData", app);
            if(METHOD(IOChannel, writeData))
               M_VTBLID(IOChannel, writeData) = METHOD(IOChannel, writeData)->vid;
         }
         CO(ImportType) = eC_findClass(app, "ImportType");
         CO(Meters) = eC_findClass(app, "Meters");
         if(CO(Meters))
         {
         }
         CO(Method) = eC_findClass(app, "Method");
         CO(MethodType) = eC_findClass(app, "MethodType");
         CO(MinMaxValue) = eC_findClass(app, "MinMaxValue");
         CO(NameSpace) = eC_findClass(app, "NameSpace");
         CO(ObjectInfo) = eC_findClass(app, "ObjectInfo");
         CO(ObjectNotationType) = eC_findClass(app, "ObjectNotationType");
         CO(Platform) = eC_findClass(app, "Platform");
         if(CO(Platform))
         {
            PROPERTY(Platform, char_ptr) = Class_findProperty(CO(Platform), "char *", app);
            if(PROPERTY(Platform, char_ptr))
            {
               Platform_from_char_ptr = (void *)PROPERTY(Platform, char_ptr)->Set;
               Platform_to_char_ptr = (void *)PROPERTY(Platform, char_ptr)->Get;
            }
         }
         CO(Point) = eC_findClass(app, "Point");
         CO(Pointd) = eC_findClass(app, "Pointd");
         CO(Pointf) = eC_findClass(app, "Pointf");
         CO(Property) = eC_findClass(app, "Property");
         CO(Radians) = eC_findClass(app, "Radians");
         if(CO(Radians))
         {
         }
         CO(SerialBuffer) = eC_findClass(app, "SerialBuffer");
         if(CO(SerialBuffer))
         {
            METHOD(SerialBuffer, free) = Class_findMethod(CO(SerialBuffer), "Free", app);
            if(METHOD(SerialBuffer, free))
               SerialBuffer_free = (void (*)(C(SerialBuffer)))METHOD(SerialBuffer, free)->function;

            PROPERTY(SerialBuffer, buffer) = Class_findProperty(CO(SerialBuffer), "buffer", app);
            if(PROPERTY(SerialBuffer, buffer))
            {
               SerialBuffer_set_buffer = (void *)PROPERTY(SerialBuffer, buffer)->Set;
               SerialBuffer_get_buffer = (void *)PROPERTY(SerialBuffer, buffer)->Get;
            }

            PROPERTY(SerialBuffer, size) = Class_findProperty(CO(SerialBuffer), "size", app);
            if(PROPERTY(SerialBuffer, size))
            {
               SerialBuffer_set_size = (void *)PROPERTY(SerialBuffer, size)->Set;
               SerialBuffer_get_size = (void *)PROPERTY(SerialBuffer, size)->Get;
            }
         }
         CO(Size) = eC_findClass(app, "Size");
         CO(StaticString) = eC_findClass(app, "StaticString");
         CO(StringAllocType) = eC_findClass(app, "StringAllocType");
         CO(SubModule) = eC_findClass(app, "SubModule");
         CO(TemplateMemberType) = eC_findClass(app, "TemplateMemberType");
         CO(TemplateParameterType) = eC_findClass(app, "TemplateParameterType");
         CO(ZString) = eC_findClass(app, "ZString");
         if(CO(ZString))
         {
            METHOD(ZString, concat) = Class_findMethod(CO(ZString), "concat", app);
            if(METHOD(ZString, concat))
               ZString_concat = (void (*)(C(ZString), C(ZString)))METHOD(ZString, concat)->function;

            METHOD(ZString, concatf) = Class_findMethod(CO(ZString), "concatf", app);
            if(METHOD(ZString, concatf))
               ZString_concatf = (void (*)(C(ZString), const char *, ...))METHOD(ZString, concatf)->function;

            METHOD(ZString, concatn) = Class_findMethod(CO(ZString), "concatn", app);
            if(METHOD(ZString, concatn))
               ZString_concatn = (void (*)(C(ZString), C(ZString), int))METHOD(ZString, concatn)->function;

            METHOD(ZString, concatx) = Class_findMethod(CO(ZString), "concatx", app);
            if(METHOD(ZString, concatx))
               ZString_concatx = (void (*)(C(ZString), typed_object_class_ptr, const void *, ...))METHOD(ZString, concatx)->function;

            METHOD(ZString, copy) = Class_findMethod(CO(ZString), "copy", app);
            if(METHOD(ZString, copy))
               ZString_copy = (void (*)(C(ZString), C(ZString)))METHOD(ZString, copy)->function;

            METHOD(ZString, copyString) = Class_findMethod(CO(ZString), "copyString", app);
            if(METHOD(ZString, copyString))
               ZString_copyString = (void (*)(C(ZString), const char *, int))METHOD(ZString, copyString)->function;

            PROPERTY(ZString, char_ptr) = Class_findProperty(CO(ZString), "char *", app);
            if(PROPERTY(ZString, char_ptr))
            {
               ZString_from_char_ptr = (void *)PROPERTY(ZString, char_ptr)->Set;
               ZString_to_char_ptr = (void *)PROPERTY(ZString, char_ptr)->Get;
            }

            PROPERTY(ZString, String) = Class_findProperty(CO(ZString), "String", app);
            if(PROPERTY(ZString, String))
            {
               ZString_from_String = (void *)PROPERTY(ZString, String)->Set;
               ZString_to_String = (void *)PROPERTY(ZString, String)->Get;
            }

            PROPERTY(ZString, string) = Class_findProperty(CO(ZString), "string", app);
            if(PROPERTY(ZString, string))
            {
               ZString_set_string = (void *)PROPERTY(ZString, string)->Set;
               ZString_get_string = (void *)PROPERTY(ZString, string)->Get;
            }
         }



         // Set up all the function pointers, ...

         FUNCTION(qsortr) = eC_findFunction(app, "qsortr");
         if(FUNCTION(qsortr))
            F(qsortr) = (void *)FUNCTION(qsortr)->function;

         FUNCTION(qsortrx) = eC_findFunction(app, "qsortrx");
         if(FUNCTION(qsortrx))
            F(qsortrx) = (void *)FUNCTION(qsortrx)->function;

         FUNCTION(archiveOpen) = eC_findFunction(app, "ArchiveOpen");
         if(FUNCTION(archiveOpen))
            F(archiveOpen) = (void *)FUNCTION(archiveOpen)->function;

         FUNCTION(archiveQuerySize) = eC_findFunction(app, "ArchiveQuerySize");
         if(FUNCTION(archiveQuerySize))
            F(archiveQuerySize) = (void *)FUNCTION(archiveQuerySize)->function;

         FUNCTION(changeWorkingDir) = eC_findFunction(app, "ChangeWorkingDir");
         if(FUNCTION(changeWorkingDir))
            F(changeWorkingDir) = (void *)FUNCTION(changeWorkingDir)->function;

         FUNCTION(copySystemPath) = eC_findFunction(app, "CopySystemPath");
         if(FUNCTION(copySystemPath))
            F(copySystemPath) = (void *)FUNCTION(copySystemPath)->function;

         FUNCTION(copyUnixPath) = eC_findFunction(app, "CopyUnixPath");
         if(FUNCTION(copyUnixPath))
            F(copyUnixPath) = (void *)FUNCTION(copyUnixPath)->function;

         FUNCTION(createTemporaryDir) = eC_findFunction(app, "CreateTemporaryDir");
         if(FUNCTION(createTemporaryDir))
            F(createTemporaryDir) = (void *)FUNCTION(createTemporaryDir)->function;

         FUNCTION(createTemporaryFile) = eC_findFunction(app, "CreateTemporaryFile");
         if(FUNCTION(createTemporaryFile))
            F(createTemporaryFile) = (void *)FUNCTION(createTemporaryFile)->function;

         FUNCTION(deleteFile) = eC_findFunction(app, "DeleteFile");
         if(FUNCTION(deleteFile))
            F(deleteFile) = (void *)FUNCTION(deleteFile)->function;

         FUNCTION(dualPipeOpen) = eC_findFunction(app, "DualPipeOpen");
         if(FUNCTION(dualPipeOpen))
            F(dualPipeOpen) = (void *)FUNCTION(dualPipeOpen)->function;

         FUNCTION(dualPipeOpenEnv) = eC_findFunction(app, "DualPipeOpenEnv");
         if(FUNCTION(dualPipeOpenEnv))
            F(dualPipeOpenEnv) = (void *)FUNCTION(dualPipeOpenEnv)->function;

         FUNCTION(dualPipeOpenEnvf) = eC_findFunction(app, "DualPipeOpenEnvf");
         if(FUNCTION(dualPipeOpenEnvf))
            F(dualPipeOpenEnvf) = (void *)FUNCTION(dualPipeOpenEnvf)->function;

         FUNCTION(dualPipeOpenf) = eC_findFunction(app, "DualPipeOpenf");
         if(FUNCTION(dualPipeOpenf))
            F(dualPipeOpenf) = (void *)FUNCTION(dualPipeOpenf)->function;

         FUNCTION(dumpErrors) = eC_findFunction(app, "DumpErrors");
         if(FUNCTION(dumpErrors))
            F(dumpErrors) = (void *)FUNCTION(dumpErrors)->function;

         FUNCTION(execute) = eC_findFunction(app, "Execute");
         if(FUNCTION(execute))
            F(execute) = (void *)FUNCTION(execute)->function;

         FUNCTION(executeEnv) = eC_findFunction(app, "ExecuteEnv");
         if(FUNCTION(executeEnv))
            F(executeEnv) = (void *)FUNCTION(executeEnv)->function;

         FUNCTION(executeWait) = eC_findFunction(app, "ExecuteWait");
         if(FUNCTION(executeWait))
            F(executeWait) = (void *)FUNCTION(executeWait)->function;

         FUNCTION(fileExists) = eC_findFunction(app, "FileExists");
         if(FUNCTION(fileExists))
            F(fileExists) = (void *)FUNCTION(fileExists)->function;

         FUNCTION(fileFixCase) = eC_findFunction(app, "FileFixCase");
         if(FUNCTION(fileFixCase))
            F(fileFixCase) = (void *)FUNCTION(fileFixCase)->function;

         FUNCTION(fileGetSize) = eC_findFunction(app, "FileGetSize");
         if(FUNCTION(fileGetSize))
            F(fileGetSize) = (void *)FUNCTION(fileGetSize)->function;

         FUNCTION(fileGetStats) = eC_findFunction(app, "FileGetStats");
         if(FUNCTION(fileGetStats))
            F(fileGetStats) = (void *)FUNCTION(fileGetStats)->function;

         FUNCTION(fileOpen) = eC_findFunction(app, "FileOpen");
         if(FUNCTION(fileOpen))
            F(fileOpen) = (void *)FUNCTION(fileOpen)->function;

         FUNCTION(fileOpenBuffered) = eC_findFunction(app, "FileOpenBuffered");
         if(FUNCTION(fileOpenBuffered))
            F(fileOpenBuffered) = (void *)FUNCTION(fileOpenBuffered)->function;

         FUNCTION(fileSetAttribs) = eC_findFunction(app, "FileSetAttribs");
         if(FUNCTION(fileSetAttribs))
            F(fileSetAttribs) = (void *)FUNCTION(fileSetAttribs)->function;

         FUNCTION(fileSetTime) = eC_findFunction(app, "FileSetTime");
         if(FUNCTION(fileSetTime))
            F(fileSetTime) = (void *)FUNCTION(fileSetTime)->function;

         FUNCTION(fileTruncate) = eC_findFunction(app, "FileTruncate");
         if(FUNCTION(fileTruncate))
            F(fileTruncate) = (void *)FUNCTION(fileTruncate)->function;

         FUNCTION(getEnvironment) = eC_findFunction(app, "GetEnvironment");
         if(FUNCTION(getEnvironment))
            F(getEnvironment) = (void *)FUNCTION(getEnvironment)->function;

         FUNCTION(getFreeSpace) = eC_findFunction(app, "GetFreeSpace");
         if(FUNCTION(getFreeSpace))
            F(getFreeSpace) = (void *)FUNCTION(getFreeSpace)->function;

         FUNCTION(getLastErrorCode) = eC_findFunction(app, "GetLastErrorCode");
         if(FUNCTION(getLastErrorCode))
            F(getLastErrorCode) = (void *)FUNCTION(getLastErrorCode)->function;

         FUNCTION(getSlashPathBuffer) = eC_findFunction(app, "GetSlashPathBuffer");
         if(FUNCTION(getSlashPathBuffer))
            F(getSlashPathBuffer) = (void *)FUNCTION(getSlashPathBuffer)->function;

         FUNCTION(getSystemPathBuffer) = eC_findFunction(app, "GetSystemPathBuffer");
         if(FUNCTION(getSystemPathBuffer))
            F(getSystemPathBuffer) = (void *)FUNCTION(getSystemPathBuffer)->function;

         FUNCTION(getWorkingDir) = eC_findFunction(app, "GetWorkingDir");
         if(FUNCTION(getWorkingDir))
            F(getWorkingDir) = (void *)FUNCTION(getWorkingDir)->function;

         FUNCTION(__e_log) = eC_findFunction(app, "Log");
         if(FUNCTION(__e_log))
            F(__e_log) = (void *)FUNCTION(__e_log)->function;

         FUNCTION(logErrorCode) = eC_findFunction(app, "LogErrorCode");
         if(FUNCTION(logErrorCode))
            F(logErrorCode) = (void *)FUNCTION(logErrorCode)->function;

         FUNCTION(__e_logf) = eC_findFunction(app, "Logf");
         if(FUNCTION(__e_logf))
            F(__e_logf) = (void *)FUNCTION(__e_logf)->function;

         FUNCTION(makeDir) = eC_findFunction(app, "MakeDir");
         if(FUNCTION(makeDir))
            F(makeDir) = (void *)FUNCTION(makeDir)->function;

         FUNCTION(makeSlashPath) = eC_findFunction(app, "MakeSlashPath");
         if(FUNCTION(makeSlashPath))
            F(makeSlashPath) = (void *)FUNCTION(makeSlashPath)->function;

         FUNCTION(makeSystemPath) = eC_findFunction(app, "MakeSystemPath");
         if(FUNCTION(makeSystemPath))
            F(makeSystemPath) = (void *)FUNCTION(makeSystemPath)->function;

         FUNCTION(moveFile) = eC_findFunction(app, "MoveFile");
         if(FUNCTION(moveFile))
            F(moveFile) = (void *)FUNCTION(moveFile)->function;

         FUNCTION(moveFileEx) = eC_findFunction(app, "MoveFileEx");
         if(FUNCTION(moveFileEx))
            F(moveFileEx) = (void *)FUNCTION(moveFileEx)->function;

         FUNCTION(removeDir) = eC_findFunction(app, "RemoveDir");
         if(FUNCTION(removeDir))
            F(removeDir) = (void *)FUNCTION(removeDir)->function;

         FUNCTION(renameFile) = eC_findFunction(app, "RenameFile");
         if(FUNCTION(renameFile))
            F(renameFile) = (void *)FUNCTION(renameFile)->function;

         FUNCTION(resetError) = eC_findFunction(app, "ResetError");
         if(FUNCTION(resetError))
            F(resetError) = (void *)FUNCTION(resetError)->function;

         FUNCTION(setEnvironment) = eC_findFunction(app, "SetEnvironment");
         if(FUNCTION(setEnvironment))
            F(setEnvironment) = (void *)FUNCTION(setEnvironment)->function;

         FUNCTION(setErrorLevel) = eC_findFunction(app, "SetErrorLevel");
         if(FUNCTION(setErrorLevel))
            F(setErrorLevel) = (void *)FUNCTION(setErrorLevel)->function;

         FUNCTION(setLoggingMode) = eC_findFunction(app, "SetLoggingMode");
         if(FUNCTION(setLoggingMode))
            F(setLoggingMode) = (void *)FUNCTION(setLoggingMode)->function;

         FUNCTION(shellOpen) = eC_findFunction(app, "ShellOpen");
         if(FUNCTION(shellOpen))
            F(shellOpen) = (void *)FUNCTION(shellOpen)->function;

         FUNCTION(unsetEnvironment) = eC_findFunction(app, "UnsetEnvironment");
         if(FUNCTION(unsetEnvironment))
            F(unsetEnvironment) = (void *)FUNCTION(unsetEnvironment)->function;

         FUNCTION(debugBreakpoint) = eC_findFunction(app, "debugBreakpoint");
         if(FUNCTION(debugBreakpoint))
            F(debugBreakpoint) = (void *)FUNCTION(debugBreakpoint)->function;

         FUNCTION(charMatchCategories) = eC_findFunction(app, "CharMatchCategories");
         if(FUNCTION(charMatchCategories))
            F(charMatchCategories) = (void *)FUNCTION(charMatchCategories)->function;

         FUNCTION(getAlNum) = eC_findFunction(app, "GetAlNum");
         if(FUNCTION(getAlNum))
            F(getAlNum) = (void *)FUNCTION(getAlNum)->function;

         FUNCTION(getCharCategory) = eC_findFunction(app, "GetCharCategory");
         if(FUNCTION(getCharCategory))
            F(getCharCategory) = (void *)FUNCTION(getCharCategory)->function;

         FUNCTION(getCombiningClass) = eC_findFunction(app, "GetCombiningClass");
         if(FUNCTION(getCombiningClass))
            F(getCombiningClass) = (void *)FUNCTION(getCombiningClass)->function;

         FUNCTION(iSO8859_1toUTF8) = eC_findFunction(app, "ISO8859_1toUTF8");
         if(FUNCTION(iSO8859_1toUTF8))
            F(iSO8859_1toUTF8) = (void *)FUNCTION(iSO8859_1toUTF8)->function;

         FUNCTION(uTF16BEtoUTF8Buffer) = eC_findFunction(app, "UTF16BEtoUTF8Buffer");
         if(FUNCTION(uTF16BEtoUTF8Buffer))
            F(uTF16BEtoUTF8Buffer) = (void *)FUNCTION(uTF16BEtoUTF8Buffer)->function;

         FUNCTION(uTF16toUTF8) = eC_findFunction(app, "UTF16toUTF8");
         if(FUNCTION(uTF16toUTF8))
            F(uTF16toUTF8) = (void *)FUNCTION(uTF16toUTF8)->function;

         FUNCTION(uTF16toUTF8Buffer) = eC_findFunction(app, "UTF16toUTF8Buffer");
         if(FUNCTION(uTF16toUTF8Buffer))
            F(uTF16toUTF8Buffer) = (void *)FUNCTION(uTF16toUTF8Buffer)->function;

         FUNCTION(uTF32toUTF8Len) = eC_findFunction(app, "UTF32toUTF8Len");
         if(FUNCTION(uTF32toUTF8Len))
            F(uTF32toUTF8Len) = (void *)FUNCTION(uTF32toUTF8Len)->function;

         FUNCTION(uTF8GetChar) = eC_findFunction(app, "UTF8GetChar");
         if(FUNCTION(uTF8GetChar))
            F(uTF8GetChar) = (void *)FUNCTION(uTF8GetChar)->function;

         FUNCTION(uTF8Validate) = eC_findFunction(app, "UTF8Validate");
         if(FUNCTION(uTF8Validate))
            F(uTF8Validate) = (void *)FUNCTION(uTF8Validate)->function;

         FUNCTION(uTF8toISO8859_1) = eC_findFunction(app, "UTF8toISO8859_1");
         if(FUNCTION(uTF8toISO8859_1))
            F(uTF8toISO8859_1) = (void *)FUNCTION(uTF8toISO8859_1)->function;

         FUNCTION(uTF8toUTF16) = eC_findFunction(app, "UTF8toUTF16");
         if(FUNCTION(uTF8toUTF16))
            F(uTF8toUTF16) = (void *)FUNCTION(uTF8toUTF16)->function;

         FUNCTION(uTF8toUTF16Buffer) = eC_findFunction(app, "UTF8toUTF16Buffer");
         if(FUNCTION(uTF8toUTF16Buffer))
            F(uTF8toUTF16Buffer) = (void *)FUNCTION(uTF8toUTF16Buffer)->function;

         FUNCTION(uTF8toUTF16BufferLen) = eC_findFunction(app, "UTF8toUTF16BufferLen");
         if(FUNCTION(uTF8toUTF16BufferLen))
            F(uTF8toUTF16BufferLen) = (void *)FUNCTION(uTF8toUTF16BufferLen)->function;

         FUNCTION(uTF8toUTF16Len) = eC_findFunction(app, "UTF8toUTF16Len");
         if(FUNCTION(uTF8toUTF16Len))
            F(uTF8toUTF16Len) = (void *)FUNCTION(uTF8toUTF16Len)->function;

         FUNCTION(accenti) = eC_findFunction(app, "accenti");
         if(FUNCTION(accenti))
            F(accenti) = (void *)FUNCTION(accenti)->function;

         FUNCTION(casei) = eC_findFunction(app, "casei");
         if(FUNCTION(casei))
            F(casei) = (void *)FUNCTION(casei)->function;

         FUNCTION(encodeArrayToString) = eC_findFunction(app, "encodeArrayToString");
         if(FUNCTION(encodeArrayToString))
            F(encodeArrayToString) = (void *)FUNCTION(encodeArrayToString)->function;

         FUNCTION(normalizeNFC) = eC_findFunction(app, "normalizeNFC");
         if(FUNCTION(normalizeNFC))
            F(normalizeNFC) = (void *)FUNCTION(normalizeNFC)->function;

         FUNCTION(normalizeNFD) = eC_findFunction(app, "normalizeNFD");
         if(FUNCTION(normalizeNFD))
            F(normalizeNFD) = (void *)FUNCTION(normalizeNFD)->function;

         FUNCTION(normalizeNFKC) = eC_findFunction(app, "normalizeNFKC");
         if(FUNCTION(normalizeNFKC))
            F(normalizeNFKC) = (void *)FUNCTION(normalizeNFKC)->function;

         FUNCTION(normalizeNFKD) = eC_findFunction(app, "normalizeNFKD");
         if(FUNCTION(normalizeNFKD))
            F(normalizeNFKD) = (void *)FUNCTION(normalizeNFKD)->function;

         FUNCTION(normalizeNFKDArray) = eC_findFunction(app, "normalizeNFKDArray");
         if(FUNCTION(normalizeNFKDArray))
            F(normalizeNFKDArray) = (void *)FUNCTION(normalizeNFKDArray)->function;

         FUNCTION(normalizeUnicode) = eC_findFunction(app, "normalizeUnicode");
         if(FUNCTION(normalizeUnicode))
            F(normalizeUnicode) = (void *)FUNCTION(normalizeUnicode)->function;

         FUNCTION(normalizeUnicodeArray) = eC_findFunction(app, "normalizeUnicodeArray");
         if(FUNCTION(normalizeUnicodeArray))
            F(normalizeUnicodeArray) = (void *)FUNCTION(normalizeUnicodeArray)->function;

         FUNCTION(stripUnicodeCategory) = eC_findFunction(app, "stripUnicodeCategory");
         if(FUNCTION(stripUnicodeCategory))
            F(stripUnicodeCategory) = (void *)FUNCTION(stripUnicodeCategory)->function;

         FUNCTION(printECONObject) = eC_findFunction(app, "PrintECONObject");
         if(FUNCTION(printECONObject))
            F(printECONObject) = (void *)FUNCTION(printECONObject)->function;

         FUNCTION(printObjectNotationString) = eC_findFunction(app, "PrintObjectNotationString");
         if(FUNCTION(printObjectNotationString))
            F(printObjectNotationString) = (void *)FUNCTION(printObjectNotationString)->function;

         FUNCTION(stringIndent) = eC_findFunction(app, "StringIndent");
         if(FUNCTION(stringIndent))
            F(stringIndent) = (void *)FUNCTION(stringIndent)->function;

         FUNCTION(writeECONObject) = eC_findFunction(app, "WriteECONObject");
         if(FUNCTION(writeECONObject))
            F(writeECONObject) = (void *)FUNCTION(writeECONObject)->function;

         FUNCTION(writeJSONObject) = eC_findFunction(app, "WriteJSONObject");
         if(FUNCTION(writeJSONObject))
            F(writeJSONObject) = (void *)FUNCTION(writeJSONObject)->function;

         FUNCTION(writeJSONObject2) = eC_findFunction(app, "WriteJSONObject2");
         if(FUNCTION(writeJSONObject2))
            F(writeJSONObject2) = (void *)FUNCTION(writeJSONObject2)->function;

         FUNCTION(writeJSONObjectMapped) = eC_findFunction(app, "WriteJSONObjectMapped");
         if(FUNCTION(writeJSONObjectMapped))
            F(writeJSONObjectMapped) = (void *)FUNCTION(writeJSONObjectMapped)->function;

         FUNCTION(writeONString) = eC_findFunction(app, "WriteONString");
         if(FUNCTION(writeONString))
            F(writeONString) = (void *)FUNCTION(writeONString)->function;

         FUNCTION(getCurrentThreadID) = eC_findFunction(app, "GetCurrentThreadID");
         if(FUNCTION(getCurrentThreadID))
            F(getCurrentThreadID) = (void *)FUNCTION(getCurrentThreadID)->function;

         FUNCTION(getRandom) = eC_findFunction(app, "GetRandom");
         if(FUNCTION(getRandom))
            F(getRandom) = (void *)FUNCTION(getRandom)->function;

         FUNCTION(getTime) = eC_findFunction(app, "GetTime");
         if(FUNCTION(getTime))
            F(getTime) = (void *)FUNCTION(getTime)->function;

         FUNCTION(randomSeed) = eC_findFunction(app, "RandomSeed");
         if(FUNCTION(randomSeed))
            F(randomSeed) = (void *)FUNCTION(randomSeed)->function;

         FUNCTION(__sleep) = eC_findFunction(app, "Sleep");
         if(FUNCTION(__sleep))
            F(__sleep) = (void *)FUNCTION(__sleep)->function;

         FUNCTION(changeCh) = eC_findFunction(app, "ChangeCh");
         if(FUNCTION(changeCh))
            F(changeCh) = (void *)FUNCTION(changeCh)->function;

         FUNCTION(changeChars) = eC_findFunction(app, "ChangeChars");
         if(FUNCTION(changeChars))
            F(changeChars) = (void *)FUNCTION(changeChars)->function;

         FUNCTION(changeExtension) = eC_findFunction(app, "ChangeExtension");
         if(FUNCTION(changeExtension))
            F(changeExtension) = (void *)FUNCTION(changeExtension)->function;

         FUNCTION(checkConsistency) = eC_findFunction(app, "CheckConsistency");
         if(FUNCTION(checkConsistency))
            F(checkConsistency) = (void *)FUNCTION(checkConsistency)->function;

         FUNCTION(checkMemory) = eC_findFunction(app, "CheckMemory");
         if(FUNCTION(checkMemory))
            F(checkMemory) = (void *)FUNCTION(checkMemory)->function;

         FUNCTION(copyBytes) = eC_findFunction(app, "CopyBytes");
         if(FUNCTION(copyBytes))
            F(copyBytes) = (void *)FUNCTION(copyBytes)->function;

         FUNCTION(copyBytesBy2) = eC_findFunction(app, "CopyBytesBy2");
         if(FUNCTION(copyBytesBy2))
            F(copyBytesBy2) = (void *)FUNCTION(copyBytesBy2)->function;

         FUNCTION(copyBytesBy4) = eC_findFunction(app, "CopyBytesBy4");
         if(FUNCTION(copyBytesBy4))
            F(copyBytesBy4) = (void *)FUNCTION(copyBytesBy4)->function;

         FUNCTION(copyString) = eC_findFunction(app, "CopyString");
         if(FUNCTION(copyString))
            F(copyString) = (void *)FUNCTION(copyString)->function;

         FUNCTION(escapeCString) = eC_findFunction(app, "EscapeCString");
         if(FUNCTION(escapeCString))
            F(escapeCString) = (void *)FUNCTION(escapeCString)->function;

         FUNCTION(fillBytes) = eC_findFunction(app, "FillBytes");
         if(FUNCTION(fillBytes))
            F(fillBytes) = (void *)FUNCTION(fillBytes)->function;

         FUNCTION(fillBytesBy2) = eC_findFunction(app, "FillBytesBy2");
         if(FUNCTION(fillBytesBy2))
            F(fillBytesBy2) = (void *)FUNCTION(fillBytesBy2)->function;

         FUNCTION(fillBytesBy4) = eC_findFunction(app, "FillBytesBy4");
         if(FUNCTION(fillBytesBy4))
            F(fillBytesBy4) = (void *)FUNCTION(fillBytesBy4)->function;

         FUNCTION(floatFromString) = eC_findFunction(app, "FloatFromString");
         if(FUNCTION(floatFromString))
            F(floatFromString) = (void *)FUNCTION(floatFromString)->function;

         FUNCTION(getActiveDesigner) = eC_findFunction(app, "GetActiveDesigner");
         if(FUNCTION(getActiveDesigner))
            F(getActiveDesigner) = (void *)FUNCTION(getActiveDesigner)->function;

         FUNCTION(getExtension) = eC_findFunction(app, "GetExtension");
         if(FUNCTION(getExtension))
            F(getExtension) = (void *)FUNCTION(getExtension)->function;

         FUNCTION(getHexValue) = eC_findFunction(app, "GetHexValue");
         if(FUNCTION(getHexValue))
            F(getHexValue) = (void *)FUNCTION(getHexValue)->function;

         FUNCTION(getLastDirectory) = eC_findFunction(app, "GetLastDirectory");
         if(FUNCTION(getLastDirectory))
            F(getLastDirectory) = (void *)FUNCTION(getLastDirectory)->function;

         FUNCTION(getRuntimePlatform) = eC_findFunction(app, "GetRuntimePlatform");
         if(FUNCTION(getRuntimePlatform))
            F(getRuntimePlatform) = (void *)FUNCTION(getRuntimePlatform)->function;

         FUNCTION(getString) = eC_findFunction(app, "GetString");
         if(FUNCTION(getString))
            F(getString) = (void *)FUNCTION(getString)->function;

         FUNCTION(getValue) = eC_findFunction(app, "GetValue");
         if(FUNCTION(getValue))
            F(getValue) = (void *)FUNCTION(getValue)->function;

         FUNCTION(isPathInsideOf) = eC_findFunction(app, "IsPathInsideOf");
         if(FUNCTION(isPathInsideOf))
            F(isPathInsideOf) = (void *)FUNCTION(isPathInsideOf)->function;

         FUNCTION(locateModule) = eC_findFunction(app, "LocateModule");
         if(FUNCTION(locateModule))
            F(locateModule) = (void *)FUNCTION(locateModule)->function;

         FUNCTION(makePathRelative) = eC_findFunction(app, "MakePathRelative");
         if(FUNCTION(makePathRelative))
            F(makePathRelative) = (void *)FUNCTION(makePathRelative)->function;

         FUNCTION(moveBytes) = eC_findFunction(app, "MoveBytes");
         if(FUNCTION(moveBytes))
            F(moveBytes) = (void *)FUNCTION(moveBytes)->function;

         FUNCTION(pathCat) = eC_findFunction(app, "PathCat");
         if(FUNCTION(pathCat))
            F(pathCat) = (void *)FUNCTION(pathCat)->function;

         FUNCTION(pathCatSlash) = eC_findFunction(app, "PathCatSlash");
         if(FUNCTION(pathCatSlash))
            F(pathCatSlash) = (void *)FUNCTION(pathCatSlash)->function;

         FUNCTION(printx) = eC_findFunction(app, "Print");
         if(FUNCTION(printx))
            F(printx) = (void *)FUNCTION(printx)->function;

         FUNCTION(printBigSize) = eC_findFunction(app, "PrintBigSize");
         if(FUNCTION(printBigSize))
            F(printBigSize) = (void *)FUNCTION(printBigSize)->function;

         FUNCTION(printBuf) = eC_findFunction(app, "PrintBuf");
         if(FUNCTION(printBuf))
            F(printBuf) = (void *)FUNCTION(printBuf)->function;

         FUNCTION(printLn) = eC_findFunction(app, "PrintLn");
         if(FUNCTION(printLn))
            F(printLn) = (void *)FUNCTION(printLn)->function;

         FUNCTION(printLnBuf) = eC_findFunction(app, "PrintLnBuf");
         if(FUNCTION(printLnBuf))
            F(printLnBuf) = (void *)FUNCTION(printLnBuf)->function;

         FUNCTION(printLnString) = eC_findFunction(app, "PrintLnString");
         if(FUNCTION(printLnString))
            F(printLnString) = (void *)FUNCTION(printLnString)->function;

         FUNCTION(printSize) = eC_findFunction(app, "PrintSize");
         if(FUNCTION(printSize))
            F(printSize) = (void *)FUNCTION(printSize)->function;

         FUNCTION(printStdArgsToBuffer) = eC_findFunction(app, "PrintStdArgsToBuffer");
         if(FUNCTION(printStdArgsToBuffer))
            F(printStdArgsToBuffer) = (void *)FUNCTION(printStdArgsToBuffer)->function;

         FUNCTION(printString) = eC_findFunction(app, "PrintString");
         if(FUNCTION(printString))
            F(printString) = (void *)FUNCTION(printString)->function;

         FUNCTION(rSearchString) = eC_findFunction(app, "RSearchString");
         if(FUNCTION(rSearchString))
            F(rSearchString) = (void *)FUNCTION(rSearchString)->function;

         FUNCTION(repeatCh) = eC_findFunction(app, "RepeatCh");
         if(FUNCTION(repeatCh))
            F(repeatCh) = (void *)FUNCTION(repeatCh)->function;

         FUNCTION(searchString) = eC_findFunction(app, "SearchString");
         if(FUNCTION(searchString))
            F(searchString) = (void *)FUNCTION(searchString)->function;

         FUNCTION(setActiveDesigner) = eC_findFunction(app, "SetActiveDesigner");
         if(FUNCTION(setActiveDesigner))
            F(setActiveDesigner) = (void *)FUNCTION(setActiveDesigner)->function;

         FUNCTION(splitArchivePath) = eC_findFunction(app, "SplitArchivePath");
         if(FUNCTION(splitArchivePath))
            F(splitArchivePath) = (void *)FUNCTION(splitArchivePath)->function;

         FUNCTION(splitDirectory) = eC_findFunction(app, "SplitDirectory");
         if(FUNCTION(splitDirectory))
            F(splitDirectory) = (void *)FUNCTION(splitDirectory)->function;

         FUNCTION(stringLikePattern) = eC_findFunction(app, "StringLikePattern");
         if(FUNCTION(stringLikePattern))
            F(stringLikePattern) = (void *)FUNCTION(stringLikePattern)->function;

         FUNCTION(stripChars) = eC_findFunction(app, "StripChars");
         if(FUNCTION(stripChars))
            F(stripChars) = (void *)FUNCTION(stripChars)->function;

         FUNCTION(stripExtension) = eC_findFunction(app, "StripExtension");
         if(FUNCTION(stripExtension))
            F(stripExtension) = (void *)FUNCTION(stripExtension)->function;

         FUNCTION(stripLastDirectory) = eC_findFunction(app, "StripLastDirectory");
         if(FUNCTION(stripLastDirectory))
            F(stripLastDirectory) = (void *)FUNCTION(stripLastDirectory)->function;

         FUNCTION(stripQuotes) = eC_findFunction(app, "StripQuotes");
         if(FUNCTION(stripQuotes))
            F(stripQuotes) = (void *)FUNCTION(stripQuotes)->function;

         FUNCTION(tokenize) = eC_findFunction(app, "Tokenize");
         if(FUNCTION(tokenize))
            F(tokenize) = (void *)FUNCTION(tokenize)->function;

         FUNCTION(tokenizeWith) = eC_findFunction(app, "TokenizeWith");
         if(FUNCTION(tokenizeWith))
            F(tokenizeWith) = (void *)FUNCTION(tokenizeWith)->function;

         FUNCTION(trimLSpaces) = eC_findFunction(app, "TrimLSpaces");
         if(FUNCTION(trimLSpaces))
            F(trimLSpaces) = (void *)FUNCTION(trimLSpaces)->function;

         FUNCTION(trimRSpaces) = eC_findFunction(app, "TrimRSpaces");
         if(FUNCTION(trimRSpaces))
            F(trimRSpaces) = (void *)FUNCTION(trimRSpaces)->function;

         FUNCTION(unescapeCString) = eC_findFunction(app, "UnescapeCString");
         if(FUNCTION(unescapeCString))
            F(unescapeCString) = (void *)FUNCTION(unescapeCString)->function;

         FUNCTION(unescapeCStringLoose) = eC_findFunction(app, "UnescapeCStringLoose");
         if(FUNCTION(unescapeCStringLoose))
            F(unescapeCStringLoose) = (void *)FUNCTION(unescapeCStringLoose)->function;

         FUNCTION(eSystem_LockMem) = eC_findFunction(app, "eSystem_LockMem");
         if(FUNCTION(eSystem_LockMem))
            F(eSystem_LockMem) = (void *)FUNCTION(eSystem_LockMem)->function;

         FUNCTION(eSystem_UnlockMem) = eC_findFunction(app, "eSystem_UnlockMem");
         if(FUNCTION(eSystem_UnlockMem))
            F(eSystem_UnlockMem) = (void *)FUNCTION(eSystem_UnlockMem)->function;

         FUNCTION(ishexdigit) = eC_findFunction(app, "ishexdigit");
         if(FUNCTION(ishexdigit))
            F(ishexdigit) = (void *)FUNCTION(ishexdigit)->function;

         FUNCTION(log2i) = eC_findFunction(app, "log2i");
         if(FUNCTION(log2i))
            F(log2i) = (void *)FUNCTION(log2i)->function;

         FUNCTION(memswap) = eC_findFunction(app, "memswap");
         if(FUNCTION(memswap))
            F(memswap) = (void *)FUNCTION(memswap)->function;

         FUNCTION(pow2i) = eC_findFunction(app, "pow2i");
         if(FUNCTION(pow2i))
            F(pow2i) = (void *)FUNCTION(pow2i)->function;

         FUNCTION(queryMemInfo) = eC_findFunction(app, "queryMemInfo");
         if(FUNCTION(queryMemInfo))
            F(queryMemInfo) = (void *)FUNCTION(queryMemInfo)->function;

         FUNCTION(strchrmax) = eC_findFunction(app, "strchrmax");
         if(FUNCTION(strchrmax))
            F(strchrmax) = (void *)FUNCTION(strchrmax)->function;

      }
   }
   else
      printf("Unable to load eC module: %s\n", ECRT_MODULE_NAME);
   return fromModule ? IPTR(fromModule, Module)->application : null;
}

C(Module) __thisModule;

