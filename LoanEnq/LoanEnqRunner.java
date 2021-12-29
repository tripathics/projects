import java.util.Scanner;

public class LoanEnqRunner {
    public static void main(String[] args) {
        // Create a new bank loan enquiry object and pass in the normal rate of interest
        BankLoanEnq enq = new BankLoanEnq(9.5f);

        // Get the borrower details from stdin and create a new borrower object
        boolean defaulter;
        String occupation;
        boolean educated;
        float income;
        float balance;
        float fd;

        
        // get the user input
        String inp;
        Scanner sc = new Scanner(System.in);

        System.out.println("\t\t-------------------------------");
        System.out.println("\t\t|     LOAN ENQUIRY SYSTEM     |");
        System.out.println("\t\t-------------------------------\n");
        System.out.println("Please enter your details: \n");
        System.out.print("1. Are you a defaulter? (Y/n): ");
        inp = sc.next();
        if (inp.equals("Y") || inp.equals("y")) {
            defaulter = true;
        }
        else {
            defaulter = false;
        }

        System.out.println("\n2. What is your occupation?");
        System.out.print("   1 for 'Govt. employee'\n   2 for 'Farmer'\n   3 for 'Others'\n   : ");
        inp = sc.next();
        if (inp.equals("1")) {
            occupation = "Govt. employee";
        } else if (inp.equals("2")) {
            occupation = "Farmer";
        } else {
            occupation = "Others";
        }
        
        System.out.print("\n3. Are you highly educated? (Y/n): ");
        inp = sc.next();
        if (inp.equals("Y") || inp.equals("y")) {
            educated = true;
        }
        else {
            educated = false;
        }

        System.out.print("\n4. Enter your annual income in Lacs: ");
        inp = sc.next();
        income = Float.parseFloat(inp);

        System.out.print("\n5. Enter your average minimum account balance (MAB) in Lacs: ");
        inp = sc.next();
        balance = Float.parseFloat(inp);

        System.out.println("\n6. Enter the amount of fixed deposit that you currently have in Lacs");
        System.out.print("   (If no FD, enter '0'): ");
        inp = sc.next();
        fd = Float.parseFloat(inp);

        sc.close();

        // Create a new borrower object
        Borrower br = new Borrower(defaulter, occupation, educated, income, balance, fd);

        // call the getROI method on enq BankLoanEnq object and pass in the borrower object
        enq.getROI(br);

        if (br.ROI == -1) {
            System.out.println("\nYou are not eligible for loan!");
        }
        else {
            System.out.println("\nCongratulations!! You are eligible for the loan at an interest of " + br.ROI + "%.");
        }
    }
}

class Borrower {
    boolean defaulter;
    String occupation;
    boolean educated;
    float income;
    float balance;
    float fd;
    float ROI;
    
    Borrower(boolean defaulter, String occupation, boolean educated, float income, float balance, float fd) {
        this.defaulter = defaulter;
        this.occupation = occupation;
        this.educated = educated;
        this.income = income;
        this.balance = balance;
        this.fd = fd;
    }
}

class BankLoanEnq {
    float normalROI;

    BankLoanEnq(float normalROI) {
        this.normalROI = normalROI;
    }

    void getROI(Borrower b) {
        // Initialize the rate of interest with the base rate of interest 
        float ROI = normalROI;

        // if borrower is defaulter, increase the ROI by 2%
        if (b.defaulter) {

            // Govt. employee is not eligible for loan
            if (b.occupation.equals("Govt. employee")) {
                b.ROI = -1;
                return;
            }
            else if (b.occupation.equals("Farmer") && b.income < 1) {
                // ROI for farmer with income less than 1 lac
                ROI = 6.0f;
            }
            else if (b.educated && b.income > 10) {
                // ROI for educated whose income is more than 10 lac
                ROI = 10.5f;
            }
            // since defaulter, raise ROI by 2.0%
            ROI += 2.0f;
        } 
        else {
            if (b.occupation.equals("Farmer") && b.income < 1) {
                // ROI for farmer with income less than 1 lac
                ROI = 6.0f;
            }
            else if (b.educated && b.income > 10) {
                ROI = 10.5f;
            }
        }

        // if borrower has min a/c balance of 10lac and fd of 50lac
        if (b.balance >= 10 && b.fd >= 50) {
            ROI -= 1.0f;
        }
        b.ROI = ROI;
    }
}