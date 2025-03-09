
#include "httplib.h"

using namespace httplib;


int main(int argc, char* argv[]){

  httplib::Server svr_;

  svr_.set_mount_point("/", "./www");
  svr_.set_mount_point("/mount", "./www2");
  svr_.set_file_extension_and_mimetype_mapping("abcde", "text/abcde");


  svr_.Get("/hi",
    [&](const Request & /*req*/, Response &res) {
      res.set_content("Hello World!", "text/plain");
    })

  .Get("/file_content",
      [&](const Request & /*req*/, Response &res) {
        res.set_file_content("/mnt/d/ABP-341.mp4");
      })

  .Get("/file_content_with_content_type",
      [&](const Request & /*req*/, Response &res) {
        res.set_file_content("./www/file", "text/plain");
      })
      
  .Get("/invalid_file_content",
      [&](const Request & /*req*/, Response &res) {
        res.set_file_content("./www/dir/invalid_file_path");
      });


  svr_.listen("0.0.0.0", 9999);


  return true;
}













