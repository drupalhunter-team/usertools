/* Copyright (c) 2014, Garrett Berg
 * This example code is released into the public domain
 *
 * Author: Garrett Berg <garrett@cloudformdesign.com>
 * 
 * Code to test the errorhandling library
 */
#define LOG_DYNAMIC
#define DEBUG

#include <SoftwareSerial.h>
#include <errorhandling.h>
#include <ByteBuffer.h>
#include <logging.h>
#include <MemoryFree.h>

SoftwareSerial SoftSerial(10, 1); // RX, TX -- 10 should be disconnected (does not receieve)
ByteBuffer failbuffer;

void setup(){
  failbuffer.init(100);
  setup_std();
  EH_test();
  //setup_soft();
  log_info("setup");
  debug("Testing Debug:");
  debug(LOGLEVEL);
  
  uint16_t mem;
  uint16_t time = micros();
  mem = freeMemory();
  time = micros() - time;
  log_info(String(time) + String("us freemem:") + String(mem));
  
}

void setup_std(){
  Serial.begin(57600);
  Serial.println("Started Std Serial");
}

void nothing(){}

/*
void setup_soft(){
  SoftSerial.begin(57600);
  SoftSerial.println("Started Soft Serial");
  L_config_soft(&SoftSerial);
}
*/

void do_failure(){
  char *errmsg = "Doing Failure";
  assert(false);
error:  
  return;
}

void do_failure2(){
  seterr(ERR_VALUE);
  if(true) iferr_log_return();
  clrerr(); // function after this should be expecting errno to exist
}

#define T1_TESTS 24
unsigned short test1(unsigned short tp){
  short tofail = false;
  short tobreak = true;
  switch(tp){
  case 0:
    tofail = true;
    assert(false);
    break;
  case 1:
    assert(true);
    break;
  case 2:
    tofail = true;
    raise(ERR_VALUE);
    break;
  case 3:
    tofail = true;
    raise(ERR_VALUE, 42, HEX);
    break;
  case 4:
    tofail = true;
    assert_raise(false, ERR_VALUE);
    break;
  case 5:
    assert_raise(true, ERR_VALUE);
    break;
  case 6:
    tofail = true;
    seterr(ERR_VALUE);
    iferr_catch();
    break;
  case 7:
    tofail = true;
    seterr(ERR_VALUE);
    iferr_log_catch();
    break;
  case 8:
    seterr(ERR_VALUE);
    clrerr();
    iferr_catch();
    break;
  case 9:
    seterr(ERR_VALUE);
    log_info("Clearing error and logging");
    clrerr_log();
    iferr_catch();
    break;
  case 10:
    tobreak = false;
    assert_return(false, 0);
    break;
  case 11:
    tobreak = true;
    assert_return(true, 1);
    break;
  case 12:
    tobreak = false;
    raise_return(ERR_VALUE, 0);
    break;
  case 13:
    tobreak = false;
    raisem_return(ERR_VALUE, 42, 0);
    break;
  case 14:
    tobreak = false;
    assert_raise_return(false, ERR_VALUE, 0);
    break;
  case 15:
    tobreak = true;
    assert_raise_return(true, ERR_VALUE, 1);
    break;
  case 16:
    tobreak = false;
    seterr(ERR_VALUE);
    //iferr_return 0;
    iferr_return(0);
    break;
  case 17:
    tobreak = false;
    seterr(ERR_VALUE);
    iferr_log_return(0);
    //iferr_log_return 0;
    break;
  case 18:
    tobreak = true;
    seterr(ERR_VALUE);
    clrerr();
    iferr_return(1);
    break;
  case 19:
    tobreak = true;
    seterr(ERR_VALUE);
    log_info("Clearing error and logging");
    clrerr_log();
    iferr_return(1);
    break;
  case 20:
    tobreak = true;
    do_failure2();
    assert(errno);
    break;
  case 21:
    // Testing syntax
    tobreak = true;
    // testing syntax
    seterr(ERR_VALUE);
    if(false) raise(ERR_VALUE);
    if(false) assert(false);
    if(false) assert_raise(false, ERR_VALUE);
    if(false) iferr_return(1);
    if(false) raise_return(ERR_VALUE, 1);
    if(false) raisem_return(ERR_VALUE, "Should not see", 1);
    if(false) assert_raise_return(false, ERR_VALUE, 1);
    if(false) assert_raisem_return(false, ERR_VALUE, "Should not see", 1);
    break;
  case 22:
    // Testing log level printing
    tobreak = true;
    
    L_println(":: You shouldn't see anything...");
    set_loglevel(LOG_SILENT);
    debug("debug");
    log_info("info");
    log_err("error");
    EH_ST_raisem(ERR_UNKNOWN);
    clrerr_log();
    
    L_println(":: Now only errors");
    set_loglevel(LOG_ERROR);
    debug("debug");
    log_info("info");
    log_err("error");
    EH_ST_raisem(ERR_UNKNOWN);
    clrerr_log();
    
    L_println(":: errors + info");
    set_loglevel(LOG_INFO);
    debug("debug");
    log_info("info");
    log_err("error");
    EH_ST_raisem(ERR_UNKNOWN);
    clrerr_log();
    
    L_println(":: should see all 3");
    set_loglevel(LOG_DEBUG);
    debug("debug");
    log_info("info");
    log_err("error");
    EH_ST_raisem(ERR_UNKNOWN);
    clrerr_log();
    
    break;
  case 23:
    L_println("Should see nothing...");
    L_set_silent(true);
    log_err("FAIL");
    log_info("FAIL");
    debug("FAIL");
    L_println("FAIL");
    L_set_silent(false);    
    break;
    
  case 24:
    tobreak = true;
    tofail = false;
    L_println("TRY CATCH");
    
    TRY(do_failure());
    CATCH(ERR_ASSERT){
    }
    else{
      assert(0);
    }
    assert(errno == 0);
    clrerr();
    
    TRY(nothing());
    CATCH_ALL{
      assert(0);
    }
    assert(errno == 0);
    
    TRY(do_failure());
    CATCH(ERR_VALUE){
      assert(0);
    }
    ELSE_CATCH(ERR_ASSERT){
    }
    else{
      assert(0);
    }
    assert(errno == 0);
    
    TRY(do_failure());
    CATCH_N(ERR_INDEX){
    }
    else{
      assert(0);
    }
    assert(errno == 0);
    
    TRY(do_failure());
    CATCH(ERR_VALUE){
      assert(0);
    }
    assert(errno == ERR_ASSERT);
    clrerr();
    
    TRY(do_failure());
    CATCH_N(ERR_ASSERT){
      assert(0);
    }
    assert(errno == ERR_ASSERT);
    clrerr();
    
  default:
    return 0;
  }
  if(tobreak) return 0;
  else return 1;

error:
  if(tofail){
    return 0;
  }
  else{
    return 1;
  }
}

void loop(){
  int out;
  int failures = 0;
  L_println("\n\n\n\n");
  L_println("Testing Standard:");
  for(int n = 0; n <= T1_TESTS; n++){
    L_flush();
    L_print("Testcase:");
    L_println(n);

    clrerr();
    out = test1(n);
    if(out){
      L_println("*** FAILURE ***");
      failures++;
      failbuffer.put(n);
    }
    else{
      L_println("*** Success ***");
    }
  }

  L_println("##### Tests Done #####");
  L_println(String("Results: ") + String(failures) + String(" failures out of: ") + String(T1_TESTS));
  if(failbuffer.getSize()) {
    L_println(String("Failed cases:"));
  }

  while(failbuffer.getSize() > 0){
    L_print((unsigned short)failbuffer.get());
    L_print(", ");
  } 
  while(true);
}



