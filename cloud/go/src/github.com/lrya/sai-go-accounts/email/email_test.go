package email

import (
	"errors"
	"io/ioutil"
	"os"
	"path/filepath"
	"reflect"
	"strings"
	"testing"
	"time"

	"fmt"

	"github.com/lrya/sai-go-accounts/util"
	"github.com/lrya/sai-go-util/jsonutil"
	"github.com/lrya/sai-go-util/log"
	"github.com/lrya/sai-go-util/postmarkapp"
	"github.com/kr/pretty"
)

func init() {
	alog.ToStdout()
}

const URL_ROOT = "https://test.lrya.com"

var tmpldir string

var testTime = time.Date(2015, 12, 1, 12, 13, 14, 0, time.UTC)

func sendEmail(msg *LryaEmail, apilocator string) (*postmarkapp.Message, error) {
	if apilocator == "" {
		apilocator = "dev"
	}
	pmclient := &postmarkapp.Client{
		LastEmail: make(chan *postmarkapp.Message, 1),
	}
	e := &LryaEmailer{TemplateDir: tmpldir, PostmarkC: pmclient, FormsUrlRoot: URL_ROOT, ApiLocator: apilocator}
	if err := e.Send(msg); err != nil {
		return nil, err
	}
	select {
	case msg := <-pmclient.LastEmail:
		return msg, nil
	case <-time.After(time.Second):
		return nil, errors.New("Timed out waiting for message")
	}
}

func init() {
	if tmpldir = os.Getenv("TEMPLATE_DIR"); tmpldir == "" {
		tmpldir, _ = os.Getwd()
		tmpldir = filepath.Join(tmpldir, "templates")
	}
}

var verificationTests = []struct {
	name             string
	emailFunc        VerificationEmailFunc
	expectedTemplate string
}{
	{"verification", VerificationEmail, "Verification"},
	{"7-day-reminder", Verification7DayReminderEmail, "Verification7dReminder"},
	{"14-day-reminder", Verification14DayReminderEmail, "Verification14dReminder"},
}

func TestLryaEmailer_VerificationEmail(t *testing.T) {
	pmclient := &postmarkapp.Client{LastEmail: make(chan *postmarkapp.Message, 1)}
	e := &LryaEmailer{TemplateDir: tmpldir, PostmarkC: pmclient, FormsUrlRoot: URL_ROOT, ApiLocator: "dev"}

	for _, test := range verificationTests {
		for _, isAdult := range []bool{true, false} {
			msg := test.emailFunc(e, "Tom", "Eliaz", "tom@example.com", AccountInfo{"USERNAME", "USERID", isAdult, "0000-00-00", "TOKEN", util.Pstring("en"), "overdrive", testTime}, nil)

			email, err := sendEmail(msg, "dev")

			if err != nil {
				t.Fatal("Error on send", err)
			}

			if email == nil {
				t.Fatal("No email created for Verification")
			}
			byt, _ := ioutil.ReadAll(email.TextBody)
			txt := string(byt)
			if !strings.Contains(txt, "https://test.lrya.com/email-verifying?user_id=USERID&token=TOKEN&dloc=dev") {
				t.Error("Email does not contain the correct URL for validation ", txt)
			}
			expectedTemplate := test.expectedTemplate
			if !isAdult {
				expectedTemplate += "Under13"
			}
			if email.Headers["X-Lrya-Tmpl"][0] != expectedTemplate {
				t.Errorf("test=%s isAdult=%t expectedTemplate=%q actual=%q", test.name, isAdult, expectedTemplate, email.Headers["X-Lrya-Tmpl"][0])
			}
		}
	}
} // end VerifyAccount

func TestLryaEmailer_VerificationEmailFrench(t *testing.T) {
	pmclient := &postmarkapp.Client{LastEmail: make(chan *postmarkapp.Message, 1)}
	e := &LryaEmailer{TemplateDir: tmpldir, PostmarkC: pmclient, FormsUrlRoot: URL_ROOT, ApiLocator: "dev"}

	for _, test := range verificationTests {
		for _, isAdult := range []bool{true, false} {
			msg := test.emailFunc(e, "Tom", "Eliaz", "tom@example.com", AccountInfo{"USERNAME", "USERID", isAdult, "0000-00-00", "TOKEN", util.Pstring("fr"), "overdrive", testTime}, nil)

			email, err := sendEmail(msg, "dev")

			if err != nil {
				t.Fatal("Error on send", err)
			}

			if email == nil {
				t.Fatal("No email created for Verification")
			}
			byt, _ := ioutil.ReadAll(email.TextBody)
			txt := string(byt)
			if !strings.Contains(txt, "https://test.lrya.com/email-verifying?user_id=USERID&token=TOKEN&dloc=dev") {
				t.Error("Email does not contain fr canary ", txt)
			}
			expectedTemplate := test.expectedTemplate
			if !isAdult {
				expectedTemplate += "Under13"
			}
			if email.Headers["X-Lrya-Tmpl"][0] != expectedTemplate {
				t.Errorf("test=%s isAdult=%t expectedTemplate=%q actual=%q", test.name, isAdult, expectedTemplate, email.Headers["X-Lrya-Tmpl"][0])
			}
		}
	}
} // end VerifyAccountFrench

func TestLryaEmailer_PostVerificationEmail(t *testing.T) {
	pmclient := &postmarkapp.Client{LastEmail: make(chan *postmarkapp.Message, 1)}
	e := &LryaEmailer{TemplateDir: tmpldir, PostmarkC: pmclient, FormsUrlRoot: URL_ROOT, ApiLocator: "dev"}

	msg := PostVerificationEmail(e, "Tom", "Eliaz", "tom@example.com", AccountInfo{"USERNAMETOM", "USERIDTOM", true, "0000-00-00", "TOKENTOM", util.Pstring("en"), "overdrive", testTime})
	email, err := sendEmail(msg, "dev")

	if err != nil {
		t.Fatal("Error on send", err)
	}

	if email == nil {
		t.Fatal("No email created for postVerification Test")
	}
	byt, _ := ioutil.ReadAll(email.TextBody)
	txt := string(byt)
	if !strings.Contains(txt, "Thank you for activating your") {
		t.Error("Email does not contain the correct child activation text ", txt)
	}
	if email.Headers["X-Lrya-Tmpl"][0] != "PostVerificationUnder13" {
		t.Fatal("Email does not have needed header PostVerificationUnder13", email.Headers)
	}
}

func TestLryaEmailer_ForgotPasswordEmail(t *testing.T) {
	pmclient := &postmarkapp.Client{LastEmail: make(chan *postmarkapp.Message, 1)}
	e := &LryaEmailer{TemplateDir: tmpldir, PostmarkC: pmclient, FormsUrlRoot: URL_ROOT, ApiLocator: "dev"}

	msg := ForgotPasswordEmail(e, "Tom", "Eliaz", "tom@example.com", []AccountInfo{{"USERNAMETOM", "USERIDTOM", true, "0000-00-00", "TOKENTOM", util.Pstring("de"), "overdrive", testTime}, {"USERNAMEBOB", "USERIDBOB", true, "0000-00-00", "TOKENBOB", util.Pstring("en"), "overdrive", testTime}}, nil)

	email, err := sendEmail(msg, "dev")

	if err != nil {
		t.Fatal("Error on send", err)
	}

	if email == nil {
		t.Fatal("No email created for forgot password")
	}
	byt, _ := ioutil.ReadAll(email.TextBody)
	txt := string(byt)
	if !strings.Contains(txt, "https://test.lrya.com/password-reset?user_id=USERIDBOB&token=TOKENBOB&dloc=") {
		t.Error("Email does not contain the correct URL for reset BOB ", txt)
	}
	if !strings.Contains(txt, "https://test.lrya.com/password-reset?user_id=USERIDTOM&token=TOKENTOM&dloc=") {
		t.Error("Email does not contain the correct URL for reset TOM ", txt)
	}
	if !strings.Contains(txt, "Select the account you wish to reset the password for") {
		t.Error("Email does not contain the correct multi-user prompt ", txt)
	}
	if email.Headers["X-Lrya-Tmpl"][0] != "ForgotPassword" {
		t.Fatal("Email does not have needed header ForgotPassword:", email.Headers)
	}
	if !strings.Contains(email.Subject, "Lrya Account Password Reset Requested") {
		t.Error("Email subject is not in English: ", email.Subject)
	}
}

func TestLryaEmailer_ForgotPasswordFrenchEmail(t *testing.T) {
	pmclient := &postmarkapp.Client{LastEmail: make(chan *postmarkapp.Message, 1)}
	e := &LryaEmailer{TemplateDir: tmpldir, PostmarkC: pmclient, FormsUrlRoot: URL_ROOT, ApiLocator: "dev"}

	msg := ForgotPasswordEmail(e, "Tom", "Eliaz", "tom@example.com", []AccountInfo{{"USERNAMETOMF", "USERIDTOMF", true, "0000-00-00", "TOKENTOMF", util.Pstring("fr"), "overdrive", testTime}, {"USERNAMEBOBF", "USERIDBOBF", true, "0000-00-00", "TOKENBOBF", util.Pstring("fr"), "overdrive", testTime}}, nil)
	email, err := sendEmail(msg, "dev")

	if err != nil {
		t.Fatal("Error on send", err)
	}

	if email == nil {
		t.Fatal("No email created for forgot password")
	}
	byt, _ := ioutil.ReadAll(email.TextBody)
	txt := string(byt)
	if !strings.Contains(txt, "Demande de réinitialisation de mot de passe") {
		t.Error("Email does not contain the correct French Text ", txt)
	}
	if !strings.Contains(txt, "https://test.lrya.com/password-reset?user_id=USERIDBOBF&token=TOKENBOBF&dloc=") {
		t.Error("Email does not contain the correct URL for reset BOB ", txt)
	}
	if !strings.Contains(txt, "https://test.lrya.com/password-reset?user_id=USERIDTOMF&token=TOKENTOMF&dloc=") {
		t.Error("Email does not contain the correct URL for reset TOM ", txt)
	}
	if !strings.Contains(txt, "Sélectionnez le compte que vous souhaitez pour lequel vous voulez reinitialiser le mot de passe") {
		t.Error("Email does not contain the correct multi-user prompt ", txt)
	}
	if email.Headers["X-Lrya-Tmpl"][0] != "ForgotPassword" {
		t.Fatal("Email does not have needed header ForgotPassword:", email.Headers)
	}
	if !strings.Contains(email.Subject, "Demande de réinitialisation de mot de passe.") {
			t.Error("Email subject is not in French: ", email.Subject)
	}
}

func TestLryaEmailer_ForgotPasswordGermanEmail(t *testing.T) {
	pmclient := &postmarkapp.Client{LastEmail: make(chan *postmarkapp.Message, 1)}
	e := &LryaEmailer{TemplateDir: tmpldir, PostmarkC: pmclient, FormsUrlRoot: URL_ROOT, ApiLocator: "dev"}

	msg := ForgotPasswordEmail(e, "Tom", "Eliaz", "tom@example.com", []AccountInfo{{"USERNAMETOM", "USERIDTOM", true, "0000-00-00", "TOKENTOM", util.Pstring("de"), "overdrive", testTime}, {"USERNAMEBOB", "USERIDBOB", true, "0000-00-00", "TOKENBOB", util.Pstring("de"), "overdrive", testTime}}, nil)
	email, err := sendEmail(msg, "dev")

	if err != nil {
		t.Fatal("Error on send", err)
	}

	if email == nil {
		t.Fatal("No email created for forgot password")
	}
	byt, _ := ioutil.ReadAll(email.TextBody)
	txt := string(byt)
	if !strings.Contains(txt, "DU HAST DARUM") {
		t.Error("Email does not contain the correct DE INNER CONTAINER COMMENT ", txt)
	}
	if !strings.Contains(txt, "https://test.lrya.com/password-reset?user_id=USERIDBOB&token=TOKENBOB&dloc=") {
		t.Error("Email does not contain the correct URL for reset BOB ", txt)
	}
	if !strings.Contains(txt, "https://test.lrya.com/password-reset?user_id=USERIDTOM&token=TOKENTOM&dloc=") {
		t.Error("Email does not contain the correct URL for reset TOM ", txt)
	}
	if !strings.Contains(txt, "Passwort für Lrya-Konto zurücksetzen") {
		t.Error("Email does not contain the correct multi-user prompt ", txt)
	}
	if email.Headers["X-Lrya-Tmpl"][0] != "ForgotPassword" {
		t.Fatal("Email does not have needed header ForgotPassword:", email.Headers)
	}
	if !strings.Contains(email.Subject, "Zurücksetzen des Passworts angefordert") {
		t.Error("Email subject is not in German: ", email.Subject)
	}
}

var locsTests = []struct {
	loc string
}{
	{"beta"},
	{"dev"},
	{""},
}

func TestDloc(t *testing.T) {
	pmclient := &postmarkapp.Client{LastEmail: make(chan *postmarkapp.Message, 1)}

	for _, test := range locsTests {

		e := &LryaEmailer{TemplateDir: tmpldir, PostmarkC: pmclient, FormsUrlRoot: URL_ROOT, ApiLocator: test.loc}
		msg := VerificationEmail(e, "Tom", "Eliaz", "tom@example.com", AccountInfo{"USERNAME", "USERID", true, "1111-11-11", "TOKEN", util.Pstring("en"), "overdrive", testTime}, nil)
		email, err := sendEmail(msg, test.loc)

		if err != nil {
			t.Fatal("Error on send", err)
		}

		if email == nil {
			t.Fatal("No email created for Verification")
		}
		byt, _ := ioutil.ReadAll(email.TextBody)
		txt := string(byt)
		if !strings.Contains(txt, fmt.Sprintf("https://test.lrya.com/email-verifying?user_id=USERID&token=TOKEN&dloc=%s", test.loc)) {
			t.Error("Email does not contain the correct URL for validation ", txt, test.loc)
		}
	}
}

func TestLryaEmailer_AccountUpdatedEmaill(t *testing.T) {
	pmclient := &postmarkapp.Client{LastEmail: make(chan *postmarkapp.Message, 1)}
	e := &LryaEmailer{TemplateDir: tmpldir, PostmarkC: pmclient, FormsUrlRoot: URL_ROOT, ApiLocator: "dev"}

	msg := AccountUpdatedEmail(e, "Tom", "Eliaz", "tom@example.com", AccountInfo{"USERNAME", "USERID", true, "1111-11-11", "TOKEN", util.Pstring("en"), "overdrive", testTime})
	email, err := sendEmail(msg, "dev")

	if err != nil {
		t.Fatal("Error on send", err)
	}

	if email == nil {
		t.Fatal("No email created for Account Updated")
	}
	byt, _ := ioutil.ReadAll(email.TextBody)
	txt := string(byt)
	if !strings.Contains(txt, "An Lrya account associated with this email address has been updated.") {
		t.Error("Email does not contain the correct test for update notice: ", txt)
	}
}

var appCreatedTests = []struct {
	app  string
	lang string
}{
	{"overdrive", "en"},
	{"overdrive", "de"},
	{"drive", "en"},
	{"web", "en"},
	{"", "en"},
	{"misc", "de"},
	{"drive", "de"},
	{"web", "de"},
	{"", "de"},
	{"misc", "de"},
}

var appCreatedFuncs = []struct {
	fname string
	f     func(e *LryaEmailer, app, lang string, adult bool) *LryaEmail
}{
	{"verification",
		func(e *LryaEmailer, app, lang string, adult bool) *LryaEmail {
			return VerificationEmail(e, "Tom", "Eliaz", "tom@example.com", AccountInfo{"USERNAME", "USERID", adult, "1111-11-11", "TOKEN", util.Pstring(lang), app, testTime}, nil)
		},
	},
	{"Verification7DayReminderEmail",
		func(e *LryaEmailer, app, lang string, adult bool) *LryaEmail {
			return Verification7DayReminderEmail(e, "Tom", "Eliaz", "tom@example.com", AccountInfo{"USERNAME", "USERID", adult, "1111-11-11", "TOKEN", util.Pstring(lang), app, testTime}, nil)
		},
	},
	{"Verification14DayReminderEmail",
		func(e *LryaEmailer, app, lang string, adult bool) *LryaEmail {
			return Verification14DayReminderEmail(e, "Tom", "Eliaz", "tom@example.com", AccountInfo{"USERNAME", "USERID", adult, "1111-11-11", "TOKEN", util.Pstring(lang), app, testTime}, nil)
		},
	},
	{"ForgotPasswordEmail",
		func(e *LryaEmailer, app, lang string, adult bool) *LryaEmail {
			return ForgotPasswordEmail(e, "Tom", "Eliaz", "tom@example.com",
				[]AccountInfo{
					{"USERNAMETOM", "USERIDTOM", adult, "0000-00-00", "TOKENTOM", util.Pstring(lang), app, testTime},
					{"USERNAMEBOB", "USERIDBOB", adult, "0000-00-00", "TOKENBOB", util.Pstring(lang), app, testTime}}, nil)
		},
	},
	{"PostVerificationEmail",
		func(e *LryaEmailer, app, lang string, adult bool) *LryaEmail {
			return PostVerificationEmail(e, "Tom", "Eliaz", "tom@example.com", AccountInfo{"USERNAME", "USERID", adult, "1111-11-11", "TOKEN", util.Pstring(lang), app, testTime})
		},
	},
	{"AccountUpdatedEmail",
		func(e *LryaEmailer, app, lang string, adult bool) *LryaEmail {
			return AccountUpdatedEmail(e, "Tom", "Eliaz", "tom@example.com", AccountInfo{"USERNAME", "USERID", adult, "1111-11-11", "TOKEN", util.Pstring(lang), app, testTime})
		},
	},
}

func TestAppCreatedEmail(t *testing.T) {
	e := &LryaEmailer{TemplateDir: tmpldir, FormsUrlRoot: URL_ROOT, ApiLocator: ""}
	for _, test := range appCreatedTests {
		for _, fun := range appCreatedFuncs {
			// Adult doesn't matter in terms of logo - but does let us run through all the different emails
			for _, adult := range []bool{true, false} {
				_, err := sendEmail(fun.f(e, test.app, test.lang, adult), "")
				if err != nil {
					t.Fatal("Error sending email", fun.fname, err)
				}
			}
		}
	}
}

type fakePostmarkBatchSend struct {
	callArgs []*postmarkapp.Message
	result   []*postmarkapp.Result
	err      error
}

func (fpm *fakePostmarkBatchSend) Send(msg *postmarkapp.Message) (*postmarkapp.Result, error) {
	panic("Not implemented")
}

func (fpm *fakePostmarkBatchSend) SendBatch(msgs []*postmarkapp.Message) ([]*postmarkapp.Result, error) {
	fpm.callArgs = msgs
	return fpm.result, fpm.err
}

var (
	stubFields = map[string]interface{}{
		"Firstname":        "first",
		"Lastname":         "last",
		"ToEmail":          "",
		"Username":         "",
		"CreatedByAppName": "",
	}
	bmsg1 = &LryaEmail{
		TemplateName: PostVerificationUnder13Template,
		ToEmail:      "send1@example.net",
		Fields:       stubFields,
	}
	bmsg2 = &LryaEmail{
		TemplateName: PostVerificationUnder13Template,
		ToEmail:      "nosend@example.com",
		Fields:       stubFields,
	}
	bmsg3 = &LryaEmail{
		TemplateName: PostVerificationUnder13Template,
		ToEmail:      "send2@example.net",
		Fields:       stubFields,
	}
	bmsg4 = &LryaEmail{
		TemplateName: PostVerificationUnder13Template,
		ToEmail:      "nosend2@example.com",
		Fields:       stubFields,
	}

	sendBatchTests = []struct {
		name      string
		msgs      []*LryaEmail
		pmresult  []*postmarkapp.Result
		pmexpargs []string // email addresses
		expected  []string
	}{
		{
			name:      "skip1",
			msgs:      []*LryaEmail{bmsg1, bmsg2, bmsg3},
			pmresult:  []*postmarkapp.Result{{Message: "bmsg1"}, {Message: "bmsg3"}},
			pmexpargs: []string{"send1@example.net", "send2@example.net"},
			expected:  []string{"send1@example.net", "nosend@example.com", "send2@example.net"},
		}, {
			name:      "all-example", // neither email should make it to postmark
			msgs:      []*LryaEmail{bmsg2, bmsg4},
			pmresult:  []*postmarkapp.Result{{Message: "bmsg2"}, {Message: "bmsg4"}},
			pmexpargs: []string{},
			expected:  []string{"nosend@example.com", "nosend2@example.com"},
		},
	}
)

func setupTempDir() string {
	d, _ := ioutil.TempDir("", "emailtest")
	enDir := filepath.Join(d, "en")
	os.Mkdir(enDir, 0777)
	ioutil.WriteFile(filepath.Join(enDir, "plain.html"), []byte("html template"), 0666)
	ioutil.WriteFile(filepath.Join(enDir, "plain.txt"), []byte("text template"), 0666)
	return d
}

func TestSendBatchOK(t *testing.T) {
	pm := &fakePostmarkBatchSend{
		result: []*postmarkapp.Result{
			{Message: "bmsg1"}, {Message: "bmsg2"}, {Message: "bmsg3"}},
		err: nil,
	}
	emailer := &LryaEmailer{
		PostmarkC: pm,
	}

	msgs := []*LryaEmail{bmsg1, bmsg2, bmsg3}
	results, err := emailer.SendBatch(msgs)
	if err != nil {
		t.Fatal("SendBatch returned error", err)
	}

	emails := make([]string, 0)
	for _, r := range results {
		emails = append(emails, r.Email.ToEmail)
	}

	expected := []string{"send1@example.net", "nosend@example.com", "send2@example.net"}
	if !reflect.DeepEqual(emails, expected) {
		pretty.Println("Expected", expected)
		pretty.Println("Result  ", emails)
		t.Error("incorrect result")
	}

	callargs := make([]string, 0)
	for _, a := range pm.callArgs {
		callargs = append(callargs, a.To[0].Address)
	}
	if !reflect.DeepEqual(callargs, expected) {
		pretty.Println("Expected", expected)
		pretty.Println("Actual  ", callargs)
		t.Error("incorrect pm args")
	}
}

func TestSendBatchErr(t *testing.T) {
	// test hard error from postmark
	err := errors.New("Test Error")
	pm := &fakePostmarkBatchSend{
		err: err,
	}

	emailer := &LryaEmailer{
		PostmarkC: pm,
	}
	results, err := emailer.SendBatch([]*LryaEmail{bmsg1, bmsg2})
	if err == nil {
		t.Fatal("No error received")
	}
	if results != nil {
		t.Fatal("Unexpected results received", results)
	}
}

func TestSendBatchSingleErr(t *testing.T) {
	// Make sure that per-email errors from postmark are returned correctly
	err := errors.New("Test Error")
	pmResponse := &postmarkapp.Result{Message: "bmsg1", ErrorCode: 123}
	pm := &fakePostmarkBatchSend{
		result: []*postmarkapp.Result{pmResponse},
	}

	emailer := &LryaEmailer{
		PostmarkC: pm,
	}
	results, err := emailer.SendBatch([]*LryaEmail{bmsg1})
	if err != nil {
		t.Fatal("Unexpected error", err)
	}
	if len(results) != 1 {
		pretty.Println("RESULTS", results)
		t.Fatal("Incorrect result count")
	}
	if !reflect.DeepEqual(results[0].Error.result, pmResponse) {
		t.Fatalf("Result did not match resposne from PM actual=%#v expected=%#v", results[0].Error.result, pmResponse)
	}
}

func TestSendErrorSupportsJsonError(t *testing.T) {
	// This will fail to compile if SendError doesn't implement
	// jsonutil.JsonError
	var _ jsonutil.JsonError = (*SendError)(nil)
}
