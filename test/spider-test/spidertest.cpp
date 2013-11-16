/*
 * Copyright (c) 2013 Morgen Matvey, Yulugin Evgeny and others.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * The names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>

#include <algorithm>

#include "common-inl.h"
#include "spidertest.h"

SpiderTest::SpiderTest() : Spider() {}

void SpiderTest::setUp() {
  CPPUNIT_ASSERT_MESSAGE("Error in reading configuration files",
                         read_database_config(&name_, &server_, &user_,
                                              &password_,
                                              "../" DATABASE_CONFIG) == 0);
}

void SpiderTest::ConstructorsTestCase() {
  // Create a simple file with list of servers
  FILE *f=fopen("../server_test", "w");
  CPPUNIT_ASSERT(f);
  CPPUNIT_ASSERT(fprintf(f, "%s\n", "test.server") > 0);
  CPPUNIT_ASSERT(fprintf(f, "%s\n", "another.test.server") > 0);
  CPPUNIT_ASSERT(fprintf(f, "\n") > 0);
  CPPUNIT_ASSERT(fprintf(f, "%s\n", "test.server") > 0);
  fclose(f);

  Spider *spider = new(std::nothrow) Spider();
  CPPUNIT_ASSERT(!spider->get_error());
  DIR *fd = opendir(TMPDIR);
  CPPUNIT_ASSERT_MESSAGE("Error in create temporary directory", fd);
  CPPUNIT_ASSERT(closedir(fd) == 0);
  delete spider;

  Spider *spider2 = new(std::nothrow) Spider("../server_test", name_, server_,
                                             user_, password_);
  CPPUNIT_ASSERT(!spider2->get_error());
  CPPUNIT_ASSERT(spider2->get_servers_file() == "../server_test");
  CPPUNIT_ASSERT(spider2->get_mime_type_attr().get_name() == "mime-type");
  std::list<std::string> test = spider2->get_servers_list();
  CPPUNIT_ASSERT_MESSAGE("test.server not readen",
                         std::find(test.begin(), test.end(),
                                   "test.server") != test.end());
  CPPUNIT_ASSERT_MESSAGE("another.test.server not readen",
                         std::find(test.begin(), test.end(),
                                   "another.test.server") != test.end());
  CPPUNIT_ASSERT_MESSAGE("Wrong number of servers", test.size() == 2);
  fd = opendir(TMPDIR);
  CPPUNIT_ASSERT_MESSAGE("Error in create temporary directory", fd);
  CPPUNIT_ASSERT(closedir(fd) == 0);
  delete spider2;
}

void SpiderTest::GetSetTestCase() {
  SpiderTest spider;
  CPPUNIT_ASSERT(!spider.get_error());

  spider.set_servers_file("some_file");
  CPPUNIT_ASSERT(spider.get_servers_file() == "some_file");

  spider.set_servers_file("some_new_file");
  CPPUNIT_ASSERT_MESSAGE("Error in set_servers_file",
                         spider.get_servers_file() == "some_new_file");
}

void SpiderTest::ReadServersListTestCase() {
  SpiderTest spider;
  CPPUNIT_ASSERT_MESSAGE("Error in constructor.", !spider.get_error());

  spider.set_servers_file("../server_test");
  CPPUNIT_ASSERT(!spider.ReadServersList());
  std::list<std::string> test = spider.get_servers_list();

  CPPUNIT_ASSERT_MESSAGE("test.server not readen",
                         std::find(test.begin(), test.end(),
                                   "test.server") != test.end());
  CPPUNIT_ASSERT_MESSAGE("another.test.server not readen",
                         std::find(test.begin(), test.end(),
                                   "another.test.server") != test.end());
  CPPUNIT_ASSERT_MESSAGE("Wrong number of servers", test.size() == 2);
  test.clear();

  // Try to read servers_file_ again.
  CPPUNIT_ASSERT(!spider.ReadServersList());
  test = spider.get_servers_list();

  CPPUNIT_ASSERT_MESSAGE("test.server not readen",
                         std::find(test.begin(), test.end(),
                                   "test.server") != test.end());
  CPPUNIT_ASSERT_MESSAGE("another.test.server not readen",
                         std::find(test.begin(), test.end(),
                                   "another.test.server") != test.end());
  CPPUNIT_ASSERT_MESSAGE("Wrong number of servers", test.size() == 2);
}

void SpiderTest::DumpToFileTestCase() {
  std::vector<std::string> path;
  path.push_back("test0");
  path.push_back("test1");

  CPPUNIT_ASSERT(!DumpToFile("test", &path));

  FILE *f = fopen("test", "r");
  CPPUNIT_ASSERT_MESSAGE("Input file not opened", f);

  char *buf = NULL;
  size_t size = 0;
  while (getline(&buf, &size, f) != -1) {
    std::string test(buf);
    test.erase(test.end()-1);  // Delete '\n' symbol
    CPPUNIT_ASSERT_MESSAGE("Error in comparing dumped file with search resault",
                           std::find(path.begin(), path.end(),
                                     test) != path.end());
  }
  CPPUNIT_ASSERT_MESSAGE("Wrong number of files", path.size() == 2);

  fclose(f);
  free(buf);
}

void SpiderTest::GetSMBDirContentTestCase() {
  std::string dir("smb://helena.ilab.mipt.ru/incoming/mipt-smb-search-test");
  std::shared_ptr<std::list<std::string> > list = GetSMBDirContent(dir);

  CPPUNIT_ASSERT_MESSAGE("Error in GetSMBDirContent", list);
  CPPUNIT_ASSERT_MESSAGE("Wrong size", list->size() == 2);
  CPPUNIT_ASSERT_MESSAGE("test_file not found",
                         std::find(list->begin(), list->end(),
                                   dir + "/test_file") != list->end());
  CPPUNIT_ASSERT_MESSAGE("test_folder not found",
                         std::find(list->begin(), list->end(),
                                   dir + "/test_folder") != list->end());
}

void SpiderTest::ScanSMBDirTestCase() {
  SpiderTest spider;
  CPPUNIT_ASSERT_MESSAGE("Error in constructor.", !spider.get_error());
  spider.set_db_name(name_);
  spider.set_db_password(password_);
  spider.set_db_server(server_);
  spider.set_db_user(user_);

  CPPUNIT_ASSERT(!spider.ConnectToDataBase());
  std::string dir("smb://helena.ilab.mipt.ru/incoming/mipt-smb-search-test");

  CPPUNIT_ASSERT(!spider.ScanSMBDir(dir));
  std::vector<std::string> files = spider.get_result();
  CPPUNIT_ASSERT_MESSAGE("test_file not found", files[0] == dir + "/test_file");
  CPPUNIT_ASSERT_MESSAGE("test_folder/test_file not found",
                         files[1] == dir + "/test_folder/test_file");
  CPPUNIT_ASSERT_MESSAGE("Wrong number of serach elements",
                         *get_last() == files[2]);
}

void SpiderTest::NameParserTestCase() {
  std::string str("some_short_string");
  std::string empthy_str;
  CPPUNIT_ASSERT(!NameParser(&str));
  CPPUNIT_ASSERT_MESSAGE("Wrong parsing", !strcmp(str.c_str(),
                                                  "some short string"));

  SpiderTest spider;
  // Try to parse empthy string
  CPPUNIT_ASSERT(spider.NameParser(&empthy_str) == -1);
  CPPUNIT_ASSERT(spider.get_error() == EINVAL);
}

void SpiderTest::DetectErrorTestCase() {
  SpiderTest spider;
  CPPUNIT_ASSERT(!spider.get_error());

  errno = ENOENT;
  spider.DetectError();
  CPPUNIT_ASSERT_MESSAGE("Error in DetectError", spider.get_error() == ENOENT);
}

void SpiderTest::AddServerTestCase() {
  SpiderTest spider;
  CPPUNIT_ASSERT(!spider.get_error());

  spider.set_servers_file("../server_test");

  CPPUNIT_ASSERT(!spider.ReadServersList());

  std::string name("some.server");
  CPPUNIT_ASSERT(!spider.AddServer(name));

  // Check that new server added in servers_list_.
  std::list<std::string> test = spider.get_servers_list();
  CPPUNIT_ASSERT_MESSAGE("test.server not readen",
                         std::find(test.begin(), test.end(),
                                   "test.server") != test.end());
  CPPUNIT_ASSERT_MESSAGE("another.test.server not readen",
                         std::find(test.begin(), test.end(),
                                   "another.test.server") != test.end());
  CPPUNIT_ASSERT_MESSAGE("some.server not in servers_list_",
                         std::find(test.begin(), test.end(),
                                   "some.server") != test.end());
  CPPUNIT_ASSERT_MESSAGE("Wrong number of servers", test.size() == 3);

  // Check that new server added in servers_file_.
  FILE *fin = fopen(spider.get_servers_file().c_str(), "r");
  CPPUNIT_ASSERT_MESSAGE("servers_file_ can't be open", fin);

  char *buf = NULL;
  size_t size = 0;
  std::string test_str;
  while (getline(&buf, &size, fin) != -1) {
    test_str.insert(0, buf);
    test_str.erase(test_str.end()-1);  // Delete '\n' symbol
    if (test_str.empty())
      continue;  // Ignore empthy strings.

    CPPUNIT_ASSERT_MESSAGE("Error in comparing dumped file with search resault",
                           std::find(test.begin(), test.end(),
                                     test_str) != test.end());
    test_str.clear();
  }
  CPPUNIT_ASSERT_MESSAGE("Wrong number of files", test.size() == 3);

  free(buf);
  fclose(fin);
}

void SpiderTest::DelServerTestCase() {
  SpiderTest spider;
  CPPUNIT_ASSERT(!spider.get_error());
  spider.set_servers_file("../server_test");

  CPPUNIT_ASSERT(!spider.ReadServersList());

  std::string name("some.server");
  CPPUNIT_ASSERT(!spider.DelServer(name));

  // Check that server deleted from servers_list_.
  std::list<std::string> test = spider.get_servers_list();
  CPPUNIT_ASSERT_MESSAGE("test.server not readen",
                         std::find(test.begin(), test.end(),
                                   "test.server") != test.end());
  CPPUNIT_ASSERT_MESSAGE("another.test.server not readen",
                         std::find(test.begin(), test.end(),
                                   "another.test.server") != test.end());
  CPPUNIT_ASSERT_MESSAGE("Wrong number of servers", test.size() == 2);

  // Check that server deleted from servers_file_.
  FILE *fin = fopen(spider.get_servers_file().c_str(), "r");
  CPPUNIT_ASSERT_MESSAGE("servers_file_ can't be open", fin);

  char *buf = NULL;
  size_t size = 0;
  while (getline(&buf, &size, fin) != -1) {
    std::string test_str(buf);
    test_str.erase(test_str.end()-1);  // Delete '\n' symbol
    CPPUNIT_ASSERT_MESSAGE("Error in comparing dumped file with search result",
                           std::find(test.begin(), test.end(),
                                     test_str) != test.end());
  }
  CPPUNIT_ASSERT_MESSAGE("Wrong number of files", test.size() == 2);

  free(buf);
  fclose(fin);
}

void SpiderTest::AddFileEntryInDataBaseTestCase() {
  SpiderTest spider;
  CPPUNIT_ASSERT(!spider.get_error());
  spider.set_db_name(name_);
  spider.set_db_server(server_);
  spider.set_db_user(user_);
  spider.set_db_password(password_);

  CPPUNIT_ASSERT(!spider.InitMimeTypeAttr());

  std::string server("some.server");
  std::string name("file");
  std::string path = "path/to/" + name;
  std::string file = "smb://" + server + "/" + path;

  struct timeval current_time;
  gettimeofday(&current_time, NULL);

  CPPUNIT_ASSERT(!spider.AddFileEntryInDataBase(file, server));

  auto db_file = FileEntry::GetByPathOnServer(path, server);
  CPPUNIT_ASSERT_MESSAGE("No such entry in data base", db_file);
  CPPUNIT_ASSERT_MESSAGE("FileEntry name", name == db_file->get_name());
  CPPUNIT_ASSERT_MESSAGE("FileEntry path", path == db_file->get_file_path());
  CPPUNIT_ASSERT_MESSAGE("FileEntry server",
                         server == db_file->get_server_name());
  CPPUNIT_ASSERT_MESSAGE("FileEntry timestamp",
                         current_time.tv_sec <= db_file->get_timestamp());
  auto param = FileParameter::GetByFileAndAttribute(
      *db_file, spider.get_mime_type_attr());
  CPPUNIT_ASSERT_MESSAGE("No such attribute", param);
  CPPUNIT_ASSERT_MESSAGE("Wrong number of attributes", param->size() == 1);
}

void SpiderTest::DumpToDataBaseTestCase() {
  SpiderTest spider;
  CPPUNIT_ASSERT(!spider.get_error());
  spider.set_db_name(name_);
  spider.set_db_server(server_);
  spider.set_db_user(user_);
  spider.set_db_password(password_);

  CPPUNIT_ASSERT(!spider.InitMimeTypeAttr());

  // Try to dump an empty vector
  CPPUNIT_ASSERT(!spider.DumpToDataBase());

  spider.AddSMBFile("smb://some.server/path/to/file1");
  spider.AddSMBFile("smb://some.server/path/to/file2");

  struct timeval current_time;
  gettimeofday(&current_time, NULL);

  CPPUNIT_ASSERT(!spider.DumpToDataBase());

  auto db_file = FileEntry::GetByPathOnServer("path/to/file1", "some.server");
  CPPUNIT_ASSERT_MESSAGE("No such entry in data base", db_file);
  CPPUNIT_ASSERT_MESSAGE("FileEntry name", "file1" == db_file->get_name());
  CPPUNIT_ASSERT_MESSAGE("FileEntry path",
                         "path/to/file1" == db_file->get_file_path());
  CPPUNIT_ASSERT_MESSAGE("FileEntry server",
                         "some.server" == db_file->get_server_name());
  CPPUNIT_ASSERT_MESSAGE("FileEntry timestamp",
                         current_time.tv_sec <= db_file->get_timestamp());

  db_file = FileEntry::GetByPathOnServer("path/to/file2", "some.server");
  CPPUNIT_ASSERT_MESSAGE("FileEntry name", "file2" == db_file->get_name());
  CPPUNIT_ASSERT_MESSAGE("FileEntry path",
                         "path/to/file2" == db_file->get_file_path());
  CPPUNIT_ASSERT_MESSAGE("FileEntry server",
                         "some.server" == db_file->get_server_name());
  CPPUNIT_ASSERT_MESSAGE("FileEntry timestamp",
                         current_time.tv_sec <= db_file->get_timestamp());
}

void SpiderTest::DetectMimeTypeTestCase() {
  SpiderTest spider;

  const char *type = spider.DetectMimeType("smb://helena.ilab.mipt.ru/"
                                           "incoming/mipt-smb-search-test");
  CPPUNIT_ASSERT_MESSAGE("Error in DetectMimeType", !spider.get_error());
  CPPUNIT_ASSERT_MESSAGE("Directory not recognized",
                         !strcmp(type, "inode/directory"));

  type = spider.DetectMimeType("smb://helena.ilab.mipt.ru/"
                               "incoming/mipt-smb-search.pdf");
  CPPUNIT_ASSERT_MESSAGE("Error in DetectMimeType", !spider.get_error());
  CPPUNIT_ASSERT_MESSAGE("PDF file not recognized",
                         !strcmp(type, "application/pdf"));
}
