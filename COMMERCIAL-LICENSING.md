# LyntraNet Commercial Licensing

**Document status:** Proposed commercial policy — not a binding license agreement  
**Project:** LyntraNet  
**Copyright holder:** Akadals

This document describes the intended commercial licensing approach for LyntraNet. It is published for transparency and discussion. It does not itself grant a commercial license, create a payment obligation, or constitute an offer capable of acceptance. Commercial rights are granted only by a separate written agreement signed by the copyright holder and the licensee.

## 1. Licensing model

LyntraNet is intended to use a dual-licensing model:

1. **Community Edition:** GNU Affero General Public License version 3.0 only (AGPL-3.0-only), as provided in the repository's `LICENSE` file.
2. **Commercial License:** A separate license for customers who need permissions or contractual terms that differ from AGPL-3.0, including certain proprietary distribution and source-code confidentiality needs.

AGPL-3.0 permits commercial use. A person or company may use, modify, provide services with, and distribute the Community Edition when they comply with the applicable AGPL-3.0 requirements. AGPL-3.0 does not require payment of a royalty to Akadals merely because LyntraNet is used commercially or made available as a network service.

A separate commercial license is an alternative licensing path. It is not an additional mandatory charge on users who choose and comply with AGPL-3.0.

## 2. Intended use cases

### 2.1 Community and evaluation use

Personal learning, research, evaluation, and open-source development may use the Community Edition under AGPL-3.0. Commercial activity is not automatically excluded, but all applicable AGPL-3.0 conditions still apply.

### 2.2 Managed online hosting — recommended deployment direction

As LyntraNet's hosting capabilities mature, the project intends to recommend a managed online hosting service for production teams that prefer not to operate networking infrastructure themselves.

A future managed service may provide some combination of:

- Server instance provisioning and lifecycle management;
- Deployment and configuration tools;
- Monitoring, logs, and operational visibility;
- Scaling and regional deployment options;
- Maintenance and managed updates.

Managed hosting would be offered under its own service terms and pricing. This repository does not currently promise that such a service is available.

Using a third-party cloud provider (for example, renting virtual machines, containers, or bare-metal servers) is still self-hosting when the customer operates and manages the LyntraNet deployment. It is not automatically considered use of a LyntraNet-managed hosting service.

### 2.3 Self-hosted commercial products

A team may operate LyntraNet on infrastructure it controls, including infrastructure rented from a cloud provider. It can either:

- Use the Community Edition and comply with AGPL-3.0; or
- Obtain a separate commercial license if it needs rights or obligations different from AGPL-3.0.

A commercial royalty applies only when it is expressly included in a signed commercial agreement. There is no automatic royalty for self-hosting under AGPL-3.0.

## 3. Indicative self-hosted commercial royalty schedule

For planning purposes, the proposed starting point for a revenue-based commercial license is a **marginal, annual tier schedule**:

| Annual net revenue from each licensed LyntraNet product or service | Indicative royalty on the revenue in that band |
| --- | ---: |
| First KRW 100,000,000 | 0% |
| Portion above KRW 100,000,000 up to KRW 500,000,000 | 2% |
| Portion above KRW 500,000,000 | 5% |

The percentages apply only to the portion of revenue inside each band, not retroactively to all revenue. The schedule is illustrative and subject to negotiation; it becomes binding only if incorporated into a signed commercial agreement.

### Example

If a licensed product has annual net revenue of KRW 600,000,000, the indicative calculation would be:

- First KRW 100,000,000 × 0% = KRW 0
- Next KRW 400,000,000 × 2% = KRW 8,000,000
- Remaining KRW 100,000,000 × 5% = KRW 5,000,000
- **Illustrative annual royalty: KRW 13,000,000**

This example is not an invoice, quote, or payment demand.

## 4. Revenue scope and calculation

The intended royalty base is the revenue attributable to the specific game, application, server product, or service identified in the commercial agreement that uses LyntraNet. It is **not** automatically the licensee's total company-wide revenue.

The signed agreement must define the calculation in detail, including:

- The licensed product(s) and service(s);
- Whether revenue is measured on a gross or net basis;
- Permitted deductions, such as VAT/sales taxes, refunds, chargebacks, and third-party platform or payment processing fees;
- Treatment of subscriptions, downloadable content, bundles, advertising, in-game purchases, and service contracts;
- The accounting period, currency, exchange-rate method, and applicable thresholds;
- Reporting format, reporting frequency, payment deadline, and record retention;
- Any minimum fee, cap, threshold, or territory-specific terms.

Unless the agreement explicitly says otherwise, revenue from unrelated products, unrelated services, and the licensee's other business activities is outside the royalty base.

The proposed thresholds are annual and measured separately for each licensed product or service. The signed agreement must specify how related editions, bundles, successor products, and multiple deployments are treated.

## 5. Commercial license scope

A commercial agreement should identify:

- The LyntraNet version, modules, and components covered;
- The permitted products, services, platforms, and territories;
- Whether modification, distribution, sublicensing, or redistribution is permitted;
- Whether source-code confidentiality is permitted and under what conditions;
- The license term, renewal, and termination conditions;
- Applicable fees and any revenue-based royalty;
- Support, maintenance, update, and security-fix commitments, if any.

No trademark, logo, third-party component, or separately licensed dependency right is granted unless the relevant rights holder expressly grants it.

## 6. Managed hosting and commercial licensing are separate

A future managed hosting subscription would pay for the hosting service described in its service agreement. It should not be confused with a royalty on the licensee's game revenue.

If a customer uses a managed hosting service, the hosting fee and any separate software license obligations must be stated clearly in the applicable agreements. There should be no double charge for the same item unless the contract expressly and transparently provides for it.

## 7. Existing AGPL rights

A separate commercial agreement does not retroactively revoke permissions already granted to recipients under AGPL-3.0. Recipients who received a copy under AGPL-3.0 retain the rights granted by that license, subject to its terms.

## 8. Copyright, ownership, and contributions

The copyright holder may offer code under a separate commercial license only to the extent they own the necessary rights or have obtained sufficient licensing authority.

Third-party contributions remain subject to their respective copyright and license terms. Merging a contribution into the repository does not, by itself, give the project permission to relicense that contribution under proprietary commercial terms.

Before accepting external contributions for dual licensing, the project should establish clear contributor terms and obtain the rights needed for the intended licensing model. Until those terms are published, contributors should not assume their contributions will be relicensed commercially.

## 9. Commercial inquiries

For an initial inquiry, open a GitHub issue titled **Commercial Licensing Inquiry**. Do not post confidential business, financial, security, or personal information in a public issue. Negotiation and execution of any agreement should take place through an appropriate private channel.

## 10. Legal status

This page is a statement of proposed policy, not legal advice and not a substitute for a negotiated contract. The royalty schedule and all commercial terms must be reviewed and expressly accepted in a signed agreement. The final agreement should be reviewed by qualified legal counsel in the relevant jurisdiction.
