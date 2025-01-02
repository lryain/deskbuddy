package jdocssvc

// doc_specs.go - Defines the default document specs for deployment.
//
// XXX: Per-document CRUD permissions are not yet enforced, so the "AllowXyz"
// arrays are blank for now.

const defaultDocSpecs = `
{
  "Specs": [
    {
      "DocName":        "mate.Entitlements",
      "Description":    "Track promotions, eg from KickStarter campaign",
      "TableName":      "Acct-mate",
      "HasGDPRData":    false,
      "ExpectedMaxLen": 1024,
      "AllowCreate":    [],
      "AllowRead":      [],
      "AllowUpdate":    [],
      "AllowDelete":    []
    },
    {
      "DocName":        "mate.RobotOwners",
      "Description":    "History of accounts that have owned this robot",
      "TableName":      "Thng-mate",
      "HasGDPRData":    false,
      "ExpectedMaxLen": 1024,
      "AllowCreate":    [],
      "AllowRead":      [],
      "AllowUpdate":    [],
      "AllowDelete":    []
    },
    {
      "DocName":        "mate.AppTokens",
      "Description":    "Tokens that authorize external apps (eg SDK) to use Victor",
      "TableName":      "AcctThng-mate",
      "HasGDPRData":    false,
      "ExpectedMaxLen": 2048,
      "AllowCreate":    [],
      "AllowRead":      [],
      "AllowUpdate":    [],
      "AllowDelete":    []
    }
  ]
}
`
